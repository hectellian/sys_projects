/**
 * @file waiter.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAX_Q 10

#define SMH "smh"
#define SERVE "s"
#define COOK "c"
#define MUTEX "m"
#define DONE "d"

int main(int argc, char *argv[]) {
    // exit condition
    int condition = 0;

     // Seting up semaphores
    sem_t *serve = sem_open(SERVE, O_CREAT, 0666, 0);
    sem_t *cook = sem_open(COOK, O_CREAT, 0666, 3); // max nb on shelve
    sem_t *mutex = sem_open(MUTEX, O_CREAT, 0666, 1); // course condition
    sem_t *done = sem_open(DONE, O_CREAT, 0666, 0);

    // Shared memory
    int fd = shm_open(SMH, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
        fprintf(stderr, "Couldn't create a shared memory: %s\n", strerror(errno));
    
    // Memory size
    if (ftruncate(fd, sizeof(int)) == -1)
        fprintf(stderr, "Couldn't modify size of memory: %s\n", strerror(errno));

    // File mapping
    int *smh = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (smh == MAP_FAILED)
        fprintf(stderr, "Couldn't map memory: %s\n", strerror(errno));

    do {
        sem_wait(serve);
        sleep(rand()%2+1); // one pizza every 1 or 2 seconds

        sem_wait(mutex);

        // decrement
        (*smh)--;

        sem_post(mutex);

        // print what's happening
        printf("Serving pizza... %d left on the shelve\n", *smh);

        sem_post(cook);

        sem_getvalue(done, &condition);

    } while (condition == 0 || *smh != 0);

    // Closing everything thats open
    sem_close(cook);
    sem_close(serve);
    sem_close(mutex);
    sem_close(done);

    // unlink named semaphores
    shm_unlink(SERVE);
    shm_unlink(COOK);
    shm_unlink(MUTEX);
    shm_unlink(DONE);

    // unmaping the file
    munmap(smh, sizeof(int));

    // closing semaphore
    close(fd);

    // unlinking semaphore
    shm_unlink(SMH);

    return 0;
}