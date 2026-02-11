#ifndef BUILTINS_H
#define BUILTINS_H

#include "core/context.h"

// cd command
// args[0] is "cd", args[1] is the target path
void builtin_cd(char **args);

// pwd command
// prints the current working directory to stdout
void builtin_pwd();

// exit command
// Sets the is_running flag in the context to false
void builtin_exit(ShellContext *ctx);

// help command
// displays a list of supported commands and usage info
void builtin_help();

// export command

#endif