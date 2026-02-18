#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "execution/executor.h"

/*
    Executes commands separated by pipes.
    Example input:
    args = {"ls", "-l", "|", "grep", ".c", "|", "wc", "-l", NULL}
*/

static int count_commands(char **args)
{
    int count = 1;
    for (int i = 0; args[i]; i++)
        if (strcmp(args[i], "|") == 0)
            count++;
    return count;
}

static char **extract_command(char **args, int start, int end)
{
    int size = end - start;
    char **cmd = malloc(sizeof(char *) * (size + 1));
    if (!cmd)
        return NULL;

    for (int i = 0; i < size; i++)
        cmd[i] = args[start + i];

    cmd[size] = NULL;
    return cmd;
}

void execute_pipeline(char **args)
{
    int cmd_count = count_commands(args);
    int prev_fd = -1;
    int i = 0;
    int cmd_index = 0;

    while (cmd_index < cmd_count)
    {
        int pipe_fd[2];
        int start = i;

        while (args[i] && strcmp(args[i], "|") != 0)
            i++;

        char **cmd = extract_command(args, start, i);

        if (cmd_index < cmd_count - 1)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (cmd_index < cmd_count - 1)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            if (execvp(cmd[0], cmd) == -1)
                perror("mysh");

            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (prev_fd != -1)
            close(prev_fd);

        if (cmd_index < cmd_count - 1)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }

        free(cmd);
        i++; 
        cmd_index++;
    }

    while (wait(NULL) > 0);
}
