#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <sys/types.h>

/* --- 1. Constants and Data Structures --- */

#define MAX_BG_JOBS 100

typedef struct {
    pid_t pid;
    int job_id;
    char *command_name;
} BackgroundJob;

typedef struct {
    bool is_running;
    BackgroundJob jobs[MAX_BG_JOBS];
    int job_count;
} ShellContext;

/* --- 2. Core Shell Management (main.c) --- */

ShellContext* context_create();
void context_free(ShellContext *ctx);
void shell_loop(ShellContext *ctx);

/* --- 3. Parsing (parser.c) --- */

char **parse_input(char *line);
void free_args(char **args); // Corrected return type to void from char

/* --- 4. Execution & Jobs (executor.c) --- */

void executor(ShellContext *ctx, char **args);
void handle_redirection(char **args);
void handle_background(char **args);
void reap_background_jobs(void);
int is_background(char **args);

/* --- 5. Built-in Commands (builtins.c) --- */

void builtin_cd(char **args);
void builtin_pwd();
void builtin_help();
void builtin_exit(ShellContext *ctx);

#endif