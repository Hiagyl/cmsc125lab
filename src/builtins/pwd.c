#include "builtins/builtins.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

// the system didn't define PATH_MAX for me, so defining it here just in case it doesn't work for anyone else
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

void builtin_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}