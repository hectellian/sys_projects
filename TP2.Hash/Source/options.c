#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "options.h" 

int check_options(int argc, char * argv[], Options *options) {
    int opt;

    while ((opt = getopt(argc, argv, "ft:")) != -1) {
        switch (opt) {
            case 'f':
                options->is_file = 1; //eq 1 if flag is activated -> files
                break;
            case 't':
                //takes the value of the argument
                options->hashname = optarg;
                break;
            default:
                //if '?' is returned:
                fprintf(stderr, "Usage: %s [-f] <To hash> [-t hash method] \n",
                argv[0]);
                exit(EXIT_FAILURE);
            }
        }

    //If the number of options is greater than the number of total arguments
    //Like this we need the string or a filename to hash
    if (optind >= argc) { 

    fprintf(stderr, "Expected a string or file to hash\n");
    exit(EXIT_FAILURE);
    }

    return 0;
}
