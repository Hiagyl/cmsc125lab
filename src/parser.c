#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100

// helper to check if a character is one of the redirection operators
int is_operator(char c) {
    return (c == '>' || c == '<' || c == '&');
}

char* preprocess_line(char *line) {
    // create a new buffer that is twice as large to account for extra spaces
    char *new_line = malloc(strlen(line) * 2 + 1);
    int j = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        // if an operator is found, add spaces around it
        if (is_operator(line[i])) {
            
            // handle the ">>" case (don't put a space between them)
            if (line[i] == '>' && line[i+1] == '>') {
                new_line[j++] = ' ';
                new_line[j++] = '>';
                new_line[j++] = '>';
                new_line[j++] = ' ';
                i++; // skip the second '>'
            } else {
                new_line[j++] = ' ';
                new_line[j++] = line[i];
                new_line[j++] = ' ';
            }
        } else {
            new_line[j++] = line[i];
        }
    }
    new_line[j] = '\0';
    return new_line;
}

char **parse_input(char *line){
    // pad redirection operators with spaces
    char *padded_line = preprocess_line(line);

    char **tokens = malloc(sizeof(char *) * MAX_TOKENS);
    if (!tokens) {
        perror("malloc");
        return NULL;
    }

    int i = 0;
    char *token = strtok(padded_line, " \t");

    while (token != NULL && i < MAX_TOKENS - 1) {
        tokens[i] = strdup(token);   // duplicate token safely

        // made redundant by padding, commented out for double-checking
        // if (!tokens[i]) {
        //     perror("strdup");
        //     // free already allocated memory
        //     for (int j = 0; j < i; j++)
        //         free(tokens[j]);
        //     free(tokens);
        //     return NULL;
        // }
        i++;
        token = strtok(NULL, " \t");
    }

    tokens[i] = NULL;  // very important for execvp
    
    free(padded_line); // clean up the temporary padded string

    return tokens;
}
