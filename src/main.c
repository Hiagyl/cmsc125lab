#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

ShellContext* context_create() {
    ShellContext *ctx = malloc(sizeof(ShellContext));
    if (ctx) {
        ctx->is_running = true;
        ctx->job_count = 0;
        ctx->next_job_id = 1;
    }
    return ctx;
}

void context_free(ShellContext *ctx) {
    free(ctx);
}

void add_to_history(ShellContext *ctx, const char *line) {
    if (!line || strlen(line) == 0) return;

    // If history is full, free the oldest command (index 0)
    if (ctx->history_count == MAX_HISTORY) {
        free(ctx->history[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            ctx->history[i - 1] = ctx->history[i];
        }
        ctx->history_count--;
    }

    ctx->history[ctx->history_count++] = strdup(line);
}

void shell_loop(ShellContext *ctx) {
    char line[1024];

    while (ctx->is_running) {
        printf("mysh> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) break;

        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue; 
        
        Command *cmd = parse_command(line); 

        if (cmd) {
            executor(ctx, cmd);
            free_command(cmd); // Clean up the struct after execution
        }
    }
}

int main() {
    ShellContext *ctx = context_create();
    if (!ctx) return 1;

    shell_loop(ctx);

    context_free(ctx);
    return 0;
}