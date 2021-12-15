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
#include <string.h>
#include <errno.h>
#include <signal.h>
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
        // Set all the important mask for the signals
        set_handlers();
        
        //Important variables
        int _argc;
        extern pid_t foreground_job, background_job; // fjob and bjob from jobs.c

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
                    if (checkBackground(_argc, _argv) != 1) { // Checks if it's not a bg job
                        foreground_job = pid;
                        int status;
                        int stat = waitpid(pid, &status, 0);
                        if (stat > 0) {
                            if (WIFSIGNALED(status)) {
                                printf("Foreground job interupted by signal\n");
                            }
                            printf("Foreground job exited with code %d\n", status);
                            foreground_job = 0;
                        } else if (stat == -1) {
                            printf("Something wrong happened (see code %d)\n", status);
                        }
                    } else if (checkBackground(_argc, _argv) == 1) {
                        background_job = pid;
                    }
                } else if (pid == 0) {
                    if (checkBackground(_argc, _argv) == 1) {
                        // Remove '&'
                        _argv[_argc - 1] = NULL;

                        // Redirect output to null
                        int fdout = open("/dev/null", O_RDWR);
                        int fdo = dup2(fdout, STDIN_FILENO);
                        printf("%d", fdo);
                        close(fdout);

                        // Not stopping background task with Ctrl+C
                        sigset_t back_mask;
                        sigemptyset(&back_mask);
                        sigaddset(&back_mask, SIGINT);
                        sigprocmask(SIG_BLOCK, &back_mask, NULL);

                    }
                    int exe = execvp(_argv[0], _argv);
                    if ( exe == -1) {
                        fprintf(stderr, "%sshell: %s: command not found\n", getenv("USER"), _argv[0]);
                        exit(EXIT_FAILURE);
                    } else {
                        exit(EXIT_SUCCESS);
                    }
                } else {
                    fprintf(stderr, "Couldn't execute processus");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}