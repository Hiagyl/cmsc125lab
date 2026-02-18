#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "core/context.h"

void executor(ShellContext *ctx, char **args);

void handle_redirection(char **args);

#endif
