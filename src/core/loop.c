#include "core/loop.h"
#include "core/context.h"
#include "builtins/builtins.h"  // now loop.c can "see" builtin_cd, etc.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

// uncomment these when implemented
// #include "parsing/parser.h" 
// #include "execution/executor.h"

// adding these since parser and executor are still disconnected
char** parse_command(char *line);
void executor(ShellContext *ctx, char **args);

void shell_loop(ShellContext *ctx) {
    char line[1024];

    while (ctx->is_running) {
        // cleanup background jobs (reap zombies)
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0);

        // print the interactive prompt
        printf("mysh> ");
        fflush(stdout);

        // read and parse input
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break; 
        }
        line[strcspn(line, "\n")] = 0;  // remove newline

        if (strlen(line) == 0) continue;    // skip empty input
        
        char **args = parse_command(line); 

        if (args != NULL && args[0] != NULL) {
            // the executor will handle fork/exec OR call your built-ins
            executor(ctx, args);
        }
    }
}