
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "core/context.h"
#include "builtins/builtins.h"
#include "execution/executor.h"

// Executor function
void executor(ShellContext *ctx, char **args) {
    if (args[0] == NULL) return;

    // Handle built-in commands first
    if (strcmp(args[0], "cd") == 0) {
        builtin_cd(args);
        return;
    }
    if (strcmp(args[0], "pwd") == 0) {
        builtin_pwd();
        return;
    }
    if (strcmp(args[0], "help") == 0) {
        builtin_help();
        return;
    }
    if (strcmp(args[0], "exit") == 0) {
        ctx->is_running = 0;
        return;
    }

    // Otherwise, execute external command
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        handle_redirection(args);
        
        if (execvp(args[0], args) == -1) {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {
        // Parent process waits
        int status;
        waitpid(pid, &status, 0);
    }
}
