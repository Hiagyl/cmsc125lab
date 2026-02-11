#include "builtins/builtins.h"
#include <stdio.h>

void builtin_help() {
    printf("CMSC 125 - mysh\n");
    printf("Type program names and arguments, then hit enter.\n");
    printf("The following are built-in commands:\n");
    printf("  cd [dir]  - Change the current directory\n");
    printf("  pwd       - Print the current working directory\n");
    printf("  help      - Display this help message\n");
    printf("  exit      - Terminate the shell\n");
    printf("Use the & at the end of a command to run it in the background.\n");
}