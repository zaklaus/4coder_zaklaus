// NOTE(zaklaus): basic git commands

global u8 git_commit_line[512];
global u8 git_buffer[1024];

function void
zaklaus_git_send_cmd(struct Application_Links *app, String_Const_u8 cmd)
{
    Scratch_Block scratch(app);
    String_Const_u8 out_buffer = SCu8("*git*");
    String_Const_u8 hot_directory = push_hot_directory(app, scratch);
    View_ID view = get_next_view_after_active(app, Access_Always);
    Buffer_Identifier id = buffer_identifier(out_buffer);
    exec_system_command(app, view, id, hot_directory, cmd, CLI_OverlapWithConflict|CLI_CursorAtEnd|CLI_SendEndSignal);
    lock_jump_buffer(app, out_buffer);
}

CUSTOM_COMMAND_SIG(zaklaus_git_status)
CUSTOM_DOC("Shows current git status")
{
    zaklaus_git_send_cmd(app, SCu8("git status"));
}

CUSTOM_COMMAND_SIG(zaklaus_git_commit)
CUSTOM_DOC("Creates a git commit")
{
    Query_Bar_Group group(app);
    Query_Bar bar_out = {};
    bar_out.prompt = string_u8_litexpr("Commit Message: ");
    bar_out.string = SCu8(git_commit_line, (u64)0);
    bar_out.string_capacity = sizeof(git_commit_line);
    if (query_user_string(app, &bar_out)) {
        bar_out.string.size = clamp_top(bar_out.string.size, sizeof(git_commit_line) - 1);
        git_commit_line[bar_out.string.size] = 0;
        snprintf((char*)git_buffer, sizeof(git_buffer), "git commit -m \"%s\"", git_commit_line);
        zaklaus_git_send_cmd(app, SCu8(git_buffer));
    }
}

CUSTOM_COMMAND_SIG(zaklaus_git_commit_and_push)
CUSTOM_DOC("Creates a git commit and pushes it")
{
    Query_Bar_Group group(app);
    Query_Bar bar_out = {};
    bar_out.prompt = string_u8_litexpr("Commit Message: ");
    bar_out.string = SCu8(git_commit_line, (u64)0);
    bar_out.string_capacity = sizeof(git_commit_line);
    if (query_user_string(app, &bar_out)) {
        bar_out.string.size = clamp_top(bar_out.string.size, sizeof(git_commit_line) - 1);
        git_commit_line[bar_out.string.size] = 0;
        snprintf((char*)git_buffer, sizeof(git_buffer), "git commit -m \"%s\" && git push", git_commit_line);
        zaklaus_git_send_cmd(app, SCu8(git_buffer));
    }
}

CUSTOM_COMMAND_SIG(zaklaus_git_add_any)
CUSTOM_DOC("Add any file to commit")
{
    Query_Bar_Group group(app);
    Query_Bar bar_out = {};
    bar_out.prompt = string_u8_litexpr("File Filter: ");
    bar_out.string = SCu8(git_commit_line, (u64)0);
    bar_out.string_capacity = sizeof(git_commit_line);
    if (query_user_string(app, &bar_out)) {
        bar_out.string.size = clamp_top(bar_out.string.size, sizeof(git_commit_line) - 1);
        git_commit_line[bar_out.string.size] = 0;
        snprintf((char*)git_buffer, sizeof(git_buffer), "git add %s", git_commit_line);
        zaklaus_git_send_cmd(app, SCu8(git_buffer));
    }
}

CUSTOM_COMMAND_SIG(zaklaus_git_remove_any)
CUSTOM_DOC("Restores any changed files")
{
    Query_Bar_Group group(app);
    Query_Bar bar_out = {};
    bar_out.prompt = string_u8_litexpr("File Filter: ");
    bar_out.string = SCu8(git_commit_line, (u64)0);
    bar_out.string_capacity = sizeof(git_commit_line);
    if (query_user_string(app, &bar_out)) {
        bar_out.string.size = clamp_top(bar_out.string.size, sizeof(git_commit_line) - 1);
        git_commit_line[bar_out.string.size] = 0;
        snprintf((char*)git_buffer, sizeof(git_buffer), "git restore %s", git_commit_line);
        zaklaus_git_send_cmd(app, SCu8(git_buffer));
    }
}

CUSTOM_COMMAND_SIG(zaklaus_git_unstage_any)
CUSTOM_DOC("Unstages any staged file changes")
{
    Query_Bar_Group group(app);
    Query_Bar bar_out = {};
    bar_out.prompt = string_u8_litexpr("File Filter: ");
    bar_out.string = SCu8(git_commit_line, (u64)0);
    bar_out.string_capacity = sizeof(git_commit_line);
    if (query_user_string(app, &bar_out)) {
        bar_out.string.size = clamp_top(bar_out.string.size, sizeof(git_commit_line) - 1);
        git_commit_line[bar_out.string.size] = 0;
        snprintf((char*)git_buffer, sizeof(git_buffer), "git restore --staged %s", git_commit_line);
        zaklaus_git_send_cmd(app, SCu8(git_buffer));
    }
}

CUSTOM_COMMAND_SIG(zaklaus_git_add_all)
CUSTOM_DOC("Adds all changed files to commit")
{
    zaklaus_git_send_cmd(app, SCu8("git add ."));
}

CUSTOM_COMMAND_SIG(zaklaus_git_pull)
CUSTOM_DOC("Pulls latest changes")
{
    zaklaus_git_send_cmd(app, SCu8("git pull"));
}

CUSTOM_COMMAND_SIG(zaklaus_git_push)
CUSTOM_DOC("Pushes our commits")
{
    zaklaus_git_send_cmd(app, SCu8("git push"));
}
