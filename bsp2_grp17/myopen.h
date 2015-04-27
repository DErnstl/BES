#ifndef myopen
#include <stdio.h>
#define myopen

extern FILE *mypopen(const char *command, const char *type);
extern int mypclose(FILE *stream);

#endif
