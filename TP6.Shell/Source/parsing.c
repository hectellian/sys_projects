/**
 * @file parsing.c
 * @author Anthony Christoforou (github.com/hectellian)
 * @brief Gets input and processes it
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "parsing.h"

/**
 * @brief shows shell GUI with user name, cwd, ...
 * 
 */
void printGUI(void) {
    printf("\033[0;36m"); // Cyan colorway for the next print
    printf("[%s@%s ", getenv("USER"), getenv("HOSTNAME")); // Current working directory
    printf("\033[0m"); // Resets colorway
    printf("%s", getcwd(NULL, 0));
    printf("\033[0;36m"); // Cyan colorway for the next print
    printf("]$ ");
    printf("\033[0m"); // Resets colorway
    fflush(stdout);
}

/**
 * @brief Get the Input object
 * 
 * @return char* the input of the user
 */
char* getInput(void) {
    // Allocates memory to the input (buffer)
    char* input = malloc(sizeof(char)*(MAX_LEN+1));
    if (input == NULL) {
        fprintf(stderr, "Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    // Gets user input
    fgets(input, MAX_LEN, stdin);
    input[strcspn(input, "\n")] = 0; // Removes newline character

    return input;
    //free(input);
}

/**
 * @brief Parses the input with the total number of arguments
 * 
 * @param argcount Number of arguments (like argc)
 * @param input Input to parse
 * @return char** All the input parsed word by word as an array
 */
char** parseInput(int *argcount, char* input) {
    // Allocate the space in our array
    char** cmds = malloc(sizeof(char*)*(MAX_LEN+2));
    if (cmds == NULL) {
        fprintf(stderr, "Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char* token = strtok(input, " ");
    while(token != NULL) {
        //printf("%s\n", token);
        cmds[i] = (char*)malloc(MAX_LEN);
        strncpy(cmds[i], token, MAX_LEN);
        i++;
        token = strtok(NULL, " ");
    }

    cmds[i] = NULL; // Removes the last space
    *argcount = i;
    return cmds;
    //free(cmds);
}