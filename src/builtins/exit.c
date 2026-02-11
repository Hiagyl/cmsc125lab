#include "builtins/builtins.h"
#include <stdio.h>
#include <stdlib.h>

void builtin_exit(ShellContext *ctx) {
    printf("Goodbye!\n");
    ctx->is_running = false;    // set the flag so the loop in loop.c stops
    
    // Note: any final cleanup of background jobs can be called here or in context_free.
}