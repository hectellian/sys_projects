#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>
#include "listing.h"
#include "backup.h"

int main(int argc, char *argv[]) {
    int i;
    for (i = optind; i < argc - 1; i++) {
        recursiveDir(argv[i]);    
    }
    printf("Backed up in: %s\n", argv[argc-1]);
    
}