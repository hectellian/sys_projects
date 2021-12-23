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

    int opt;
    int fflag = 0;
    int aflag = 0;

    while ((opt = getopt(argc, argv, "af")) != -1) {
        switch (opt) {
            case 'f':
                fflag = 1;
                break;
            case 'a':
                aflag = 1;
                break;
            default:
                exit(EXIT_FAILURE);
            }
        }

    if (argc - optind == 1)
        recursiveDir(argv[optind]); 

    size_t srclen = argc - optind - 1;
    char *srcfiles[srclen];
    for (int i = optind; i < argc; i++) {
        src_files[i - optind] = argv[1];
    }

    unsigned int flags = 0;
    if (aflag)
        flags |= U_AFLAG;
    

    printf("Backed up in: %s\n", argv[argc-1]);
    
    return 0;
}