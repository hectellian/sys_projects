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
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <signal.h>
#include "jobs.h"

pid_t foreground_job,  background_job;

/**
 * @brief checks if command line is to be executed as a background job
 * 
 * @param _argc argument count
 * @param _argv argument value
 * @return int true or false
 */
int checkBackground(int _argc, char* _argv[]) {
    if (strcmp(_argv[_argc - 1], "&") == 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Handles the signal input
 * 
 * @param signum SIGINT or SIGHUP, the rest is ignored
 */
void handler(int sig) {
    switch (sig)
    {
    case SIGINT: //< Redirects the signal to the principal job
        if (foreground_job > 0) {
            kill(foreground_job, SIGINT); // Sends SIGINT signal to foreground pid
        }
        break;
    
    case SIGHUP: // Redirects the signal to everything
        if (foreground_job > 0) {
            kill(foreground_job, SIGHUP);// Sends SIGHUP signal to foreground pid
        } if (background_job > 0) {
            kill(background_job, SIGHUP); // Sends SIGHUP signal to backgorun pid
        }
        exit(EXIT_SUCCESS);
        break;
    }
}

/**
 * @brief Handles the signals for the child, usually stops it
 * 
 * @param signum signal number
 * @param siginfo signal struct
 * @param unused 
 */
void child_process_signal(int signum, siginfo_t *siginfo, void* unused) {
    if (siginfo->si_pid == background_job) {
        int status;
        waitpid(siginfo->si_pid, &status, 0);
        write(STDOUT_FILENO, "\nBackground Job Exited with Success\n", 36); // Using write because of redirected output
        background_job = 0;
    }
}

/**
 * @brief Set the handlers object
 * 
 */
void set_handlers(void) {
    // SIGCHILD handler
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = child_process_signal;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &sa, NULL);

    // SIGINT and SIGUHP
    struct sigaction sig;
    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = handler;
    sig.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGHUP, &sig, NULL);

    // Ignore SIGTERM and SIGQUIT
    sigset_t pmask;
    sigemptyset(&pmask);
    sigaddset(&pmask, SIGTERM);
    sigaddset(&pmask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &pmask, NULL);
}

