#ifndef __JOBS_HEADER__
#define __JOBS_HEADER__

int checkBackground(int _argc, char* _argv[]);
void handler(int signum);
void child_process_signal(int signum, siginfo_t *siginfo, void* unused);
void set_handlers(void);

#endif