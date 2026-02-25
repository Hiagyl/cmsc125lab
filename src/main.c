#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "shell.h"

// Context Management (formerly context.c)
ShellContext* context_create() {
    ShellContext *ctx = malloc(sizeof(ShellContext));
    if (ctx) {
        ctx->is_running = true;
        ctx->job_count = 0;
    }
    return ctx;
}

void context_free(ShellContext *ctx) {
    // Note: If you add dynamic strings to jobs later, free them here
    free(ctx);
}

// Shell Loop (formerly loop.c)
void shell_loop(ShellContext *ctx) {
    char line[1024];

    while (ctx->is_running) {
        // cleanup background jobs (reap zombies)
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0); // TODO: move to an exp func

        // print the interactive prompt
        printf("mysh> ");
        fflush(stdout);

        // read input
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break; 
        }

        // remove newline character
        line[strcspn(line, "\n")] = 0;

        // skip empty input
        if (strlen(line) == 0) continue; 
        
        // parse the input into arguments
        char **args = parse_input(line); 

        if (args != NULL && args[0] != NULL) {
            // send to the executor (which handles built-ins and external apps)
            executor(ctx, args);
        }
    }
}

int main() {
    ShellContext *ctx = context_create();   // setup the shell state
    if (!ctx) {
        fprintf(stderr, "Failed to initialize shell context\n");
        return 1;
    }

    shell_loop(ctx);    // start the interactive loop 

    context_free(ctx);  // cleanup after the loop ends (after 'exit')
    
    return 0;
}