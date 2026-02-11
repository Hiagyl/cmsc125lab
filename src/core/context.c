#include "core/context.h"
#include <stdlib.h>

ShellContext* context_create() {
    ShellContext *ctx = malloc(sizeof(ShellContext));
    if (ctx) {
        ctx->is_running = true;
        ctx->job_count = 0;
    }
    return ctx;
}

void context_free(ShellContext *ctx) {
    // free the strings in the jobs array here
    free(ctx);
}