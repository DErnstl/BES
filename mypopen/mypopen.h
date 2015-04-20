#ifndef MYPOPEN_H_
#define MYPOPEN_H_

FILE *mypopen(const char *cmd, const char *type);
int mypclose(FILE *file_stream);

#endif	/* MYPOPEN_H_ */
