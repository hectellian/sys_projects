#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "backup.h"

void copy(const char* src, const char* destination) {
    int src_fd, destination_fd;
    char buf[4096];
    ssize_t nbBytes;

    src_fd = open(src, 0);
    if (src_fd < 0) {
        fprintf(stderr, "Cannot open file %s: %s\n", src, strerror(errno));
        exit(EXIT_FAILURE);
    }

    destination_fd = open(destination, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR); 
    if (destination_fd < 0) {
        fprintf(stderr, "Cannot open file %s: %s\n", destination, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ( (nbBytes = read(src_fd, buf, 4096)) > 0) {
        if(write(destination_fd, buf, nbBytes) != nbBytes)
        close(src_fd);
        close(destination_fd);
        return;
    } 

    
    
}

