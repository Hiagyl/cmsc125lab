#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "shell.h"

// standard fallback for systems where PATH_MAX isn't defined
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// cd: Change Directory
void builtin_cd(char **args) {
    // args[0] is "cd", args[1] is the target directory
    if (args[1] == NULL) {
        fprintf(stderr, "mysh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            // if chdir fails (e.g., folder doesn't exist)
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
    printf("The following are built-in commands:\n");
    printf("  cd [dir]   - Change the current directory\n");
    printf("  pwd        - Print the current working directory\n");
    printf("  help       - Display this help message\n");
    printf("  exit       - Terminate the shell\n");
    printf("Use the & at the end of a command to run it in the background.\n");
}

// exit: Terminate Shell
void builtin_exit(ShellContext *ctx) {
    printf("Goodbye!\n");
    ctx->is_running = false;    // set the flag so the loop in loop.c stops

    // Note: any final cleanup of background jobs can be called here or in context_free.
}