#ifndef __DIGEST_HEADER__
#define __DIGEST_HEADER__

/* ---------- TYPE DEF ---------- */
/* ---------- PROTOTYPES ---------- */

//calculate String hash
void EVP_String(char *message, const char *digestname);
//calculates File content hash
void EVP_File(const char *filename, const char *digestname);

#endif