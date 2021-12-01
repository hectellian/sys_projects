/**
 * @file jobs.c
 * @author Anthony Christoforou (github.com/hectellian)
 * @brief Execute non implemented commands
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "jobs.h"

int checkBackground(int _argc, char* _argv[]) {
    if (strcmp(_argv[_argc - 1], "&") == 0) {
        return 1;
    } else {
        return 0;
    }
}
