#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include "digest.h"


void EVP_String(char *message, const char *digestname) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;

    md = EVP_get_digestbyname(digestname);

    if (md == NULL) {
        printf("Unknown message digest %s\n", digestname);
        exit(EXIT_FAILURE);
    } else {
        mdctx = EVP_MD_CTX_new(); //Init the "box"
        EVP_DigestInit_ex(mdctx, md, NULL); // Init the digest
        
        EVP_DigestUpdate(mdctx, message, strlen(message)); //Add and Hash message in the "box"

        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
        EVP_MD_CTX_free(mdctx); //free memory

        for (i = 0; i < md_len; i++) //prints hash
            printf("%02x", md_value[i]);
            printf("\n");
    }
}

//Ssame as EVP_String except for few things
void EVP_File(const char *filename, const char *digestname) {
    FILE* file;
    char *message = NULL;
    size_t len = 0;

    file = fopen(filename, "r"); //open the file and check if it's readable
    if (file == NULL) { //If not we exit
        printf("file %s is not valid\n", filename);
        exit(EXIT_FAILURE);
    }

    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;

    md = EVP_get_digestbyname(digestname);

    if (md == NULL) {
        printf("Unknown message digest %s\n", digestname);
        exit(EXIT_FAILURE);
    } else {
        mdctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(mdctx, md, NULL);

        //adds all file content line by line
        while ((getline(&message, &len, file)) != -1) {
            EVP_DigestUpdate(mdctx, message, strlen(message));
        }

        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
        EVP_MD_CTX_free(mdctx);

        for (i = 0; i < md_len; i++)
            printf("%02x", md_value[i]);
            printf("\n");

        fclose(file);
    }
}
    