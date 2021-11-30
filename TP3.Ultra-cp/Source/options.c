#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "options.h" 

int check_options(int argc, char * argv[], Options *options) {
    int opt;
    int lflag, aflag;

    while ((opt = getopt(argc, argv, "af")) != -1) {
        switch (opt) {
            case 'f':
                lflag = 1;
                break;
            case 'a':
                aflag = 1;
                break;
            default:
                exit(EXIT_FAILURE);
            }
        }

    return 0;
}
