#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>
#include "listing.h"
#include "backup.h"

void make_inode(const char *path, Inode *inode) {
    struct stat info; //infos for our stat
    char time[50];
    //we call the stat function
    if (lstat(path, &info) < 0) {
        fprintf(stderr,"cannot stat %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    //if everything goes well we create our inode info
    inode->type = inode_type(path);
    inode->permission = inode_permissions(path);
    inode->size = info.st_size;
    strftime(time, sizeof(time), "%c", localtime(&info.st_mtime)); 
    inode->m_date = time;
}

char* inode_permissions(const char *path) {
    struct stat info; //infos for our stat
    char *permissions = NULL;
    permissions = (char*)calloc(10, sizeof(char));
    //we call the stat function
    if (lstat(path, &info) < 0) {
        fprintf(stderr,"cannot stat %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    //let's test every permission
    (info.st_mode & S_IRUSR) ? strcat(permissions, "r") : strcat(permissions, "-");
    (info.st_mode & S_IWUSR) ? strcat(permissions, "w") : strcat(permissions, "-");
    (info.st_mode & S_IXUSR) ? strcat(permissions, "x") : strcat(permissions, "-");
    (info.st_mode & S_IRGRP) ? strcat(permissions, "r") : strcat(permissions, "-");
    (info.st_mode & S_IWGRP) ? strcat(permissions, "w") : strcat(permissions, "-");
    (info.st_mode & S_IXGRP) ? strcat(permissions, "x") : strcat(permissions, "-");
    (info.st_mode & S_IROTH) ? strcat(permissions, "r") : strcat(permissions, "-");
    (info.st_mode & S_IWOTH) ? strcat(permissions, "w") : strcat(permissions, "-");
    (info.st_mode & S_IXOTH) ? strcat(permissions, "x") : strcat(permissions, "-");

    return permissions;
}

char* inode_type(const char *path) {
    struct stat info; //infos for our stat
    char *t;
    //we call the stat function
    if (lstat(path, &info) < 0) {
        fprintf(stderr,"cannot stat %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    //let's test every permission
    if (S_ISDIR(info.st_mode)) {
        t = "d";
    }
    else if (S_ISLNK(info.st_mode)) {
        t = "l";
    }
    else {
        t = "-";
    }

    return t;
}

void recursiveDir(char *entry) {

    struct dirent *direntry;
    DIR *dir = opendir(entry);
    const char *d_name;

    while ((direntry = readdir(dir)) != NULL) {
        Inode *inode = (Inode*)malloc(sizeof(Inode));
        char *fullpath = malloc(80000000);

        d_name = direntry->d_name;
        snprintf(fullpath, 80000000, "%s/%s", entry, d_name);

        make_inode(fullpath, inode);
        //copy(fullpath, destination);
        printf("%s%s      %zu %s %s/%s\n", inode->type, inode->permission, inode->size, inode->m_date, entry, d_name);
        
        if (strcmp(inode->type, "d") == 0) {
            if (strcmp(d_name, "..") != 0 && strcmp(d_name, ".") != 0) {
                char *newpath = malloc(80000000);

                snprintf(newpath, 80000000, "%s/%s", entry, d_name);

                recursiveDir(newpath);

                free(newpath);

            }
        }  

        free(inode); 
        free(fullpath);
    }
    closedir(dir); 
}

