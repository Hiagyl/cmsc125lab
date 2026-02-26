#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shell.h"

// Helper to add spaces around special characters for easier tokenization
char* preprocess_line(char *line) {
    char *new_line = malloc(strlen(line) * 3 + 1); // Extra space for added gaps
    if (!new_line) return NULL;

    int j = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        // Handle ">>" (two characters)
        if (line[i] == '>' && line[i+1] == '>') {
            new_line[j++] = ' ';
            new_line[j++] = '>';
            new_line[j++] = '>';
            new_line[j++] = ' ';
            i++; // Skip next '>'
        } 
        // Handle single characters: <, >, &
        else if (line[i] == '<' || line[i] == '>' || line[i] == '&') {
            new_line[j++] = ' ';
            new_line[j++] = line[i];
            new_line[j++] = ' ';
        } 
        else {
            new_line[j++] = line[i];
        }
    }
    new_line[j] = '\0';
    return new_line;
}

Command* parse_command(char *line) {
    // 1. Preprocess to add spaces
    char *formatted_line = preprocess_line(line);
    if (!formatted_line) return NULL;

    Command *cmd = calloc(1, sizeof(Command));
    if (!cmd) {
        free(formatted_line);
        return NULL;
    }

    int i = 0;
    char *token = strtok(formatted_line, " \t");

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

    free(formatted_line); // Clean up the temporary formatted string
    return cmd;
}

void free_command(Command *cmd) {
    if (!cmd) return;
    for (int i = 0; cmd->argv[i]; i++) free(cmd->argv[i]);
    if (cmd->input_file) free(cmd->input_file);
    if (cmd->output_file) free(cmd->output_file);
    free(cmd);
}