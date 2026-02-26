#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

Command* parse_command(char *line) {
    Command *cmd = calloc(1, sizeof(Command)); // calloc zeroes memory
    if (!cmd) return NULL;

    int i = 0;
    char *token = strtok(line, " \t");

    while (token != NULL && i < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t");
            if (token) cmd->input_file = strdup(token);
        } 
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t");
            if (token) {
                cmd->output_file = strdup(token);
                cmd->append = 0;
            }
        } 
        else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t");
            if (token) {
                cmd->output_file = strdup(token);
                cmd->append = 1;
            }
        } 
        else if (strcmp(token, "&") == 0) {
            cmd->background = 1;
        } 
        else {
            cmd->argv[i++] = strdup(token);
        }
        token = strtok(NULL, " \t");
    }
    cmd->argv[i] = NULL;
    return cmd;
}

void free_command(Command *cmd) {
    if (!cmd) return;
    for (int i = 0; cmd->argv[i]; i++) free(cmd->argv[i]);
    if (cmd->input_file) free(cmd->input_file);
    if (cmd->output_file) free(cmd->output_file);
    free(cmd);
}