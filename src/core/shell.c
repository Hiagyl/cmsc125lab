#include "core/shell.h"
#include "core/context.h"
#include "core/loop.h"

int shell_run() {
    ShellContext *ctx = context_create();   // setup the shell state
    shell_loop(ctx);    // start the interactive loop 
    context_free(ctx);  // cleanup after the loop ends (after 'exit')
    
    return 0;
}