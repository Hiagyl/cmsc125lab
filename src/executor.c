#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "shell.h"

// Background Job Management (formerly jobs.c)
#define MAX_JOBS 100

typedef struct {
    int job_id;
    pid_t pid;
    char command[256];
} Job;

static Job jobs[MAX_JOBS];
static int job_count = 0;
static int next_job_id = 1;

int is_background(char **args) {
    int i = 0;
    while (args[i]) i++;

    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        // remove the "&" so the OS doesn't try to execute it
        args[i - 1] = NULL;
        return 1;
    }
    return 0;
}

void reap_background_jobs(void) {
    int status;
    for (int i = 0; i < job_count; i++) {
        pid_t pid = waitpid(jobs[i].pid, &status, WNOHANG);
        if (pid > 0) {
            printf("[%d] Finished: %s (PID: %d)\n", jobs[i].job_id, jobs[i].command, jobs[i].pid);
            
            // shift array to remove the completed job
            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];
            }
            job_count--;
            i--;
        }
    }
}

// I/O Redirection (formerly redirection.c)

void handle_redirection(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        int fd = -1;

        if (strcmp(args[i], ">") == 0) {
            // Output Redirection - Overwrite ">"
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDOUT_FILENO);
        } else if (strcmp(args[i], ">>") == 0) {
            // Output Redirection - Append ">>"
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDOUT_FILENO);
        } else if (strcmp(args[i], "<") == 0) {
            // Input Redirection "<"
            fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) { perror("open"); exit(1); }
            dup2(fd, STDIN_FILENO);
        }

        if (fd != -1) {
            close(fd);
            args[i] = NULL; // terminate args at the symbol
        }
    }
}

// Main Execution Logic (formerly executor.c)

void executor(ShellContext *ctx, char **args) {
    if (args[0] == NULL) return;

    reap_background_jobs();

    // Built-ins Check
    if (strcmp(args[0], "cd") == 0) { builtin_cd(args); return; }
    if (strcmp(args[0], "pwd") == 0) { builtin_pwd(); return; }
    if (strcmp(args[0], "help") == 0) { builtin_help(); return; }
    if (strcmp(args[0], "exit") == 0) { builtin_exit(ctx); return; }

    // Background Check
    int background = is_background(args);

    // Process Creation
    pid_t pid = fork();
    if (pid == 0) {
        // Child Process
        handle_redirection(args);
        if (execvp(args[0], args) == -1) {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent Process
        if (background) {
            if (job_count < MAX_JOBS) {
                jobs[job_count].job_id = next_job_id++;
                jobs[job_count].pid = pid;
                
                // Store command string for the "Finished" message
                jobs[job_count].command[0] = '\0';
                for (int i = 0; args[i]; i++) {
                    strncat(jobs[job_count].command, args[i], 255);
                    strncat(jobs[job_count].command, " ", 255);
                }
                printf("[%d] Started: %d\n", jobs[job_count].job_id, pid);
                job_count++;
            }
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    } else {
        perror("fork");
    }
}
