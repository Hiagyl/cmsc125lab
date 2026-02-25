#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 100

char **parse_input(char *line){
    char **tokens = malloc(sizeof(char *) * MAX_TOKENS);
    if (!tokens) {
        perror("malloc");
        return NULL;
    }

    int i = 0;
    char *token = strtok(line, " \t");

    while (token != NULL && i < MAX_TOKENS - 1) {
        tokens[i] = strdup(token);   // duplicate token safely
        if (!tokens[i]) {
            perror("strdup");
            // free already allocated memory
            for (int j = 0; j < i; j++)
                free(tokens[j]);
            free(tokens);
            return NULL;
        }

        i++;
        token = strtok(NULL, " \t");
    }

    tokens[i] = NULL;  // very important for execvp
    return tokens;
}
