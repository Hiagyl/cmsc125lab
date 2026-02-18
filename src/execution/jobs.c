#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "execution/jobs.h"

#define MAX_JOBS 100

typedef struct s_job {
    int job_id;
    pid_t pid;
    char command[256];
} Job;

static Job jobs[MAX_JOBS];
static int job_count = 0;
static int next_job_id = 1;

/* ------------------------- */
/* Check if command ends with & */
/* ------------------------- */
int is_background(char **args)
{
    int i = 0;
    while (args[i])
        i++;

    if (i > 0 && strcmp(args[i - 1], "&") == 0)
    {
        free(args[i - 1]);
        args[i - 1] = NULL;
        return 1;
    }
    return 0;
}

/* ------------------------- */
/* Reap finished background jobs */
/* ------------------------- */
void reap_background_jobs(void)
{
    int status;
    pid_t pid;

    for (int i = 0; i < job_count; i++)
    {
        pid = waitpid(jobs[i].pid, &status, WNOHANG);
        if (pid > 0)
        {
            printf("[%d] Finished: %s (PID: %d)\n",
                   jobs[i].job_id,
                   jobs[i].command,
                   jobs[i].pid);

            /* Remove job from list */
            for (int j = i; j < job_count - 1; j++)
                jobs[j] = jobs[j + 1];

            job_count--;
            i--;
        }
    }
}

/* ------------------------- */
/* Handle background execution */
/* ------------------------- */
void handle_background(char **args)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        execvp(args[0], args);
        perror("mysh");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
        return;
    }

    /* Parent does NOT wait */

    if (job_count < MAX_JOBS)
    {
        jobs[job_count].job_id = next_job_id++;
        jobs[job_count].pid = pid;

        /* Store command as string */
        jobs[job_count].command[0] = '\0';
        for (int i = 0; args[i]; i++)
        {
            strcat(jobs[job_count].command, args[i]);
            strcat(jobs[job_count].command, " ");
        }

        printf("[%d] Started background job: %s(PID: %d)\n",
               jobs[job_count].job_id,
               jobs[job_count].command,
               pid);

        job_count++;
    }
}
