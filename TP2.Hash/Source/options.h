#ifndef __OPTIONS_FILE__
#define __OPTIONS_FILE__

/* ---------- TYPE DEF ---------- */

//Struct to return the hash method and if it's hashing a string ot a file
typedef struct Options {
    int is_file; //1 or 0
    char *hashname; //SHA1 by default
} Options;

/* ---------- PROTOTYPES ---------- */

//Checks options used
int check_options(int argc, char * argv[], Options *options);

#endif