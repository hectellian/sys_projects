#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <getopt.h>
#include "options.h"
#include "digest.h"


int main(int argc, char * argv[]) {
    Options options = {0, "sha1"}; //Struct for options
    int v = 0;
    //allocating memory to make something like a buffer
    char *str = (char *)malloc(v);

    check_options(argc, argv, &options); //check options
    printf("Hash method: %s, Is File: %d\n", options.hashname, options.is_file);

    
    if (options.is_file) { 
        for (int i = optind; i < argc; i++)   
            EVP_File(argv[i], options.hashname);
    } else {
        for (int i = optind; i < argc; i++) { 
            //Iterates on optind and put every argument in str   
            str = (char *)realloc(str, (v + strlen(argv[i])));
            strcat(str, argv[i]);
            //adds a space everytime
            strcat(str, " ");
        }
        str[strlen(str) - 1] = '\0';
        EVP_String(str, options.hashname);
    }

    //frees memory
    free(str);
}
