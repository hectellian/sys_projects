/**
 * @file builtin.c
 * @author Anthony Christoforou (github.com/hectellian)
 * @brief Builtin functions for a shell (exit and cd)
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
#include <signal.h>
#include "builtin.h"

/**
 * @brief change directories
 * 
 * @param argc_ number of arguments
 * @param argv_ arguments
 * @return int 
 */
void cd(char* path) {
    if (path == NULL) {
        chdir("/home/hectellian");
    } else {
        int rst = chdir(path);
        if (rst == -1) {
            fprintf(stderr, "%sshell: cd: %s: %s\n", getenv("USER"), path, strerror(errno));
        }
    }
}

/**
 * @brief exits the shell and background processes proprely
 * 
 */
void hexit(void) {
    int pid = getpid();
    printf("Exited %s shell\n", getenv("USER"));
    kill(pid, SIGHUP);
}

/**
 * @brief Executes a built-in function
 * 
 * @param _argc Number of arguments
 * @param _argv Arguments
 * @return int 
 */
int execbin(int _argc, char* _argv[]) {
    char* cmd = _argv[0];
    //printf("%s\n",cmd);
    char* cdf = "cd";
    char* hexitf = "exit";
    //printf("%s\n", _argv[1]);
    if (strcmp(cmd, cdf) == 0 && (_argc == 2 || _argc == 1)) {
        cd(_argv[1]);
    } else if (strcmp(cmd, hexitf) == 0 && _argc == 1) {
        hexit();
    } else {
        return -1;
    }

    return 0;
}
