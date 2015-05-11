#include <sem182.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Howto:
 * - parameter check (getopt)
 * - key anlegen
 * - semaphore anlegen (jeder Prozess)
 * - shm anlegen
 * - shm mounten
 * - P
 * - daten rein schreiben
 * - V
 * - shm umount
 * - EMPFAENGER: shm entfernen
 * - EMPFAENGER: semaphore entfernen */

#define SHMKEY (1000 * (int) getuid(void) + 0)
#define EMPFAENGERKEY (1000 * (int) getuid(void) + 1)
#define SENDERKEY (1000 * (int) getuid(void) + 2)

int main(int argc, const char *argv[]) {

	int opt;
	int ringbuffer;
	int mykey;
	int semid;
	int shmid;
	int *pnShm = 0;

	/* parameter abfragen */
	while ((opt = getopt(argc, argv, "m:")) != -1) {
		switch (opt) {
			case 'm':
				ringbuffer = atoi(optarg);
				break;
			default: /* '?' */
				fprintf(stderr, "Usage: %s <-m ringbuffer elements>\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	/* Ringbuffer positiv? */
	if (ringbuffer < 1) {
		fprintf(stderr, "Ringbuffer must be greater 0\n");
	}

	/* semaphore anlegen */
	if ((semid = seminit(SENDERKEY, 0600, ringbuffer)) == -1 ) {
		/* FEHLERBEHANDLUNG */
		/* -1 = semaphore existiert bereits */
	}

	/* shm anlegen */
	if ((shmid = shmget(SHMKEY, ringbuffer, 0600|IPC_CREAT)) == -1) {
		/* FEHLERBEHANDLUNG */
	}

	/* shm einhaengen */
	if ((pnShm = shmat(shmid, NULL, 0)) == (int *) -1) {
		/* FEHLERBEHANDLUNG */
	}

	/* P */
	/* daten rein schreiben */
	/* V */

	/* shm aushaengen */
	if (shmdt(pnShm) == -1 ) {
		/* FEHLERBEHANDLUNG */
	}

	return EXIT_SUCCESS;
}
