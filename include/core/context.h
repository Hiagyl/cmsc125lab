#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdbool.h>
#include <sys/types.h>

#define MAX_BG_JOBS 100

typedef struct {
    pid_t pid;
    int job_id;
    char *command_name;
} BackgroundJob;

typedef struct {
    bool is_running;    // set to false when 'exit' is typed
    BackgroundJob jobs[MAX_BG_JOBS];    // list of background PIDs
    int job_count;
} ShellContext;

ShellContext* context_create();
void context_free(ShellContext *ctx);

#endif