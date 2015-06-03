#ifndef MYBSP3_H
#define MYBSP3_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <error.h>
#include <signal.h>
#include <sem182.h>

#define SHMKEY (1000 * (int) getuid() + 0)
#define EMPFAENGERKEY (1000 * (int) getuid() + 1)
#define SENDERKEY (1000 * (int) getuid() + 2)
#define ACCESSMODE 0600

extern int semid_sender;
extern int semid_empfaenger;
extern int shmid;
extern int *shmaddr;
extern int ringbuffer;

void cleanup(void);
int del_shm(int shmid);
int del_semaphore(int semid);
int mygetopts(int argc, char* const argv[]);
int mysemaphore(int key, int seminitvalue);
void myp(int semid);
void myv(int semid);
int myshmcreate(int ringbuffer);
void myshmmount(int shmid, int readonlyflag);
void myshmumount(void);
#endif
