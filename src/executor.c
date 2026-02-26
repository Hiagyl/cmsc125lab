#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"

void apply_redirection(Command *cmd) {
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) { perror("open input"); exit(1); }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        int fd = open(cmd->output_file, flags, 0644);
        if (fd < 0) { perror("open output"); exit(1); }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void executor(ShellContext *ctx, Command *cmd) {
    if (cmd->argv[0] == NULL) return;

    reap_background_jobs(ctx);

    if (handle_builtins(ctx, cmd)) return;

    pid_t pid = fork();
    if (pid == 0) {
        apply_redirection(cmd);
        if (execvp(cmd->argv[0], cmd->argv) == -1) {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    } 
    else if (pid > 0) {
        if (cmd->background) {
            if (ctx->job_count < MAX_BG_JOBS) {
                BackgroundJob *j = &ctx->jobs[ctx->job_count];
                j->pid = pid;
                j->job_id = ctx->next_job_id++;
                strncpy(j->command_name, cmd->argv[0], 255);
                printf("[%d] %d\n", j->job_id, pid);
                ctx->job_count++;
            }
        } else {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("fork");
    }
}

void reap_background_jobs(ShellContext *ctx) {
    int status;
    for (int i = 0; i < ctx->job_count; i++) {
        pid_t pid = waitpid(ctx->jobs[i].pid, &status, WNOHANG);
        if (pid > 0) {
            printf("[%d] Finished: %s\n", ctx->jobs[i].job_id, ctx->jobs[i].command_name);
            for (int j = i; j < ctx->job_count - 1; j++) {
                ctx->jobs[j] = ctx->jobs[j + 1];
            }
            ctx->job_count--;
            i--;
        }
    }
}