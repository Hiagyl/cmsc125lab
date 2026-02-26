#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "shell.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* --- Internal Built-in Implementations --- */

// cd: Change Directory
void builtin_cd(Command *cmd) {
    if (cmd->argv[1] == NULL) {
        fprintf(stderr, "mysh: expected argument to \"cd\"\n");
    } else {
        if (chdir(cmd->argv[1]) != 0) {
            perror("mysh");
        }
    }
}

// pwd: Print Working Directory
void builtin_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("mysh: getcwd error");
    }
}

// help: Display Instructions
void builtin_help() {
    printf("CMSC 125 - mysh\n");
    printf("Type program names and arguments, then hit enter.\n");
    printf("Built-in commands:\n");
    printf("  cd [dir]   - Change the current directory\n");
    printf("  pwd        - Print the current working directory\n");
    printf("  history    - Display the last ten commmands\n");
    printf("  help       - Display this help message\n");
    printf("  exit       - Terminate the shell\n");
    printf("Use '&' for background tasks and '>', '>>', '<' for redirection.\n");
}

// exit: Terminate Shell
void builtin_exit(ShellContext *ctx) {
    ctx->is_running = false;
}

void builtin_history(ShellContext *ctx) {
    for (int i = 0; i < ctx->history_count; i++) {
        printf("%d  %s\n", i + 1, ctx->history[i]);
    }
}

/* --- Public Dispatcher --- */

int handle_builtins(ShellContext *ctx, Command *cmd) {
    if (cmd->argv[0] == NULL) return 0;

    if (strcmp(cmd->argv[0], "cd") == 0) {
        builtin_cd(cmd);
        return 1;
    }
    if (strcmp(cmd->argv[0], "pwd") == 0) {
        builtin_pwd();
        return 1;
    }
    if (strcmp(cmd->argv[0], "help") == 0) {
        builtin_help();
        return 1;
    }
    if (strcmp(cmd->argv[0], "exit") == 0) {
        builtin_exit(ctx);
        return 1;
    }
    if (strcmp(cmd->argv[0], "history") == 0) {
        builtin_history(ctx);
    return 1;}   

    return 0; // Not a builtin, let the executor handle it
}