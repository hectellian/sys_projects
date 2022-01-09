/**
 * @file cook.c
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
    // Max quantity
    int quantity = MAX_Q;

    // Seting up semaphores
    sem_t *serve = sem_open(SERVE, O_CREAT, 0666, 0);
    sem_t *cook = sem_open(COOK, O_CREAT, 0666, 3); // max nb on shelve
    sem_t *mutex = sem_open(MUTEX, O_CREAT, 0666, 1); // mutual exclusion
    sem_t *done = sem_open(DONE, O_CREAT, 0666, 0);

    // Shared memory
    int fd = shm_open(SMH, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
        fprintf(stderr, "Couldn't create a shared memory: %s\n", strerror(errno));
    
    // Memory size
    if (ftruncate(fd, sizeof(int)) == -1)
        fprintf(stderr, "Couldn't modify size of memory: %s\n", strerror(errno));

    // File mapping, init to 0
    int *smh = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (smh == MAP_FAILED)
        fprintf(stderr, "Couldn't map memory: %s\n", strerror(errno));

    printf("Let's start cooking\n");

    while (quantity != 0) {
        // lock semaphore pointed by cook
        sem_wait(cook);
        if (quantity != MAX_Q)
            printf("making a pizza...\n");
            sleep(rand()%2+1); //making a pizza
        
        // lock semaphore pointed by mutex
        sem_wait(mutex);

        // addind increment smh
        (*smh)++;

        // unlock semaphore mutex
        sem_post(mutex);

        // print what's happening
        printf("Just cooked a pizza, there's now %d pizzas on the shelve\n", *smh);

        // unlock serve
        sem_post(serve);

        // decrement value of the quantity
        quantity--;
        printf("There's %d pizza left to cook\n", quantity);

        if (*smh == 3)
            printf("Shelve is full, taking a break...\n");

    }
        sem_post(done);

        printf("All pizza are done\n");


        // Closing everything thats open
        sem_close(serve);
        sem_close(cook);
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