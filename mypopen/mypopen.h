#ifndef MYPOPEN_H_
#define MYPOPEN_H_

static pid_t child_pid;
static unsigned int limit = 0;

FILE *mypopen(const char *cmd, const char *type);
int mypclose(FILE *file_stream);

#endif	/* MYPOPEN_H_ */
