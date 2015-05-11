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
	int senderid;
	int empfaengerid;
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
		exit(EXIT_FAILURE);
	}

	/* semaphore anlegen */
	if ((senderid = seminit(SENDERKEY, 0600, ringbuffer)) == -1 ) {
		/* FEHLERBEHANDLUNG */
		/* -1 = semaphore existiert bereits */
		if ((senderid = semgrep(SENDERKEY)) == -1) {
			/* FEHLERBEHANDLUNG */
			fprintf(stderr, "semaphore error\n");
			exit(EXIT_FAILURE);
		}
	}

	/* shm anlegen */
	errno = 0;
	if ((shmid = shmget(SHMKEY, ringbuffer, 0600|IPC_CREAT|IPC_EXCL)) == -1) {
		/* FEHLERBEHANDLUNG */
		if (errno == EEXIST) {
			if ((shmid = shmget(SHMKEY, ringbuffer)) == -1) {
				fprintf(stderr, "shm error\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	/* shm einhaengen */
	errno = 0;
	if ((pnShm = shmat(shmid, NULL, 0)) == (int *) -1) {
		/* FEHLERBEHANDLUNG */
	}

	/* P(senderid) */
	/* daten rein schreiben */
	/* semaphore empfaenger holen */
	if ((empfaengerid = semgrep(EMPFAENGERKEY)) == -1) {
		/* FEHLERBEHANDLUNG */
	}
	/* V(empfaengerid) */

	/* shm aushaengen */
	if (shmdt(pnShm) == -1 ) {
		/* FEHLERBEHANDLUNG */
	}

	return EXIT_SUCCESS;
}
