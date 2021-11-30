#ifndef __LISTING_HEADER__
#define __LISTING_HEADER__
#include <unistd.h>

/* ---------- STRUCT ---------- */
typedef struct Inode {
    char* type;
    char* permission;
    size_t size;
    char* m_date;
} Inode;

/* ---------- PROTOTYPES ---------- */
void make_inode(const char *path, Inode *inode);
char* inode_type(const char *path);
char* inode_permissions(const char *path);
void recursiveDir(char *entry);

#endif