#include <stdio.h>
/**
 * @file shell.c
 * @author Anthony Christoforou (github.com/hectellian)
 * @brief main function for shell
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include "parsing.h"
#include "builtin.h"
#include "jobs.h"

int main(int argc, char* argv[], char* env[]) {
    if (argc != 1) { // Checking arguments
        fprintf(stderr, "Too much argumetns, should be: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    while (1) { // Infinite loop
        //Important variables
        int _argc;

        // GUI
        printGUI();

        // Get input
        char* input = getInput();
        // Parse Input
        char** _argv = parseInput(&_argc, input);
        
        // Check if the user just pressed enter
        if (_argc == 0) {continue;}
        else {
            int exc = execbin(_argc, _argv);
            if (exc == -1) { // Checking if it's not a bin function
                pid_t pid = fork(); // New process
                if (pid > 0) { // Parent
                    int status = 0;
                    // TODO Handle Signals
                    if (checkBackground(_argc, _argv) != 1) { // Checks if it's not a bg job
                        waitpid(pid, &status, WUNTRACED);
                    // TODO Handle background/foreground tasks
                    }
                } else if (pid == 0) {
                    int exe = execvp(_argv[0], _argv);
                    if ( exe == -1) {
                        fprintf(stderr, "Command %s not found: %s\n", _argv[0], strerror(errno));
                    }
                } else {
                    fprintf(stderr, "Couldn't execute processus");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}