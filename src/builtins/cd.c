#include "builtins/builtins.h"
#include <unistd.h>
#include <stdio.h>

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