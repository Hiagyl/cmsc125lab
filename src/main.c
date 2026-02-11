#include "core/shell.h"

int main(int argc, char **argv) {
    // These two lines "trick" the compiler into thinking they are used
    (void)argc;
    (void)argv;
    
    return shell_run();
}