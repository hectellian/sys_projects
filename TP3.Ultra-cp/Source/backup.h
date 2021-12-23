#ifndef __BACKUP_HEADER__
#define __BACKUP_HEADER__

void copy(const char* src[], size_t len, const char* destination, unsigned int flags);
void cwrapper(const char *in, const char *out, const struct *in_sb);

#endif