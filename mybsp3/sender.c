#include "mybsp3.h"
#include <sem182.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <error.h>
#include <signal.h>

/* TODO:
 * - P
 * - daten rein schreiben
 * - V
 * - aufräumen
 * - Signalbehandlung */

int main(int argc, const char *argv[]) {

	int opt;
	int ringbuffer;
	int semid;
	int shmid;
	int *pnShm = 0;

	/* parameter abfragen */
	ringbuffer = mygetopts(argc, argv);

	/* semaphore anlegen */
	semid = mysemaphore(SENDERKEY, ringbuffer);

	/* shm anlegen */
	shmid = myshmcreate(SHMKEY, ringbuffer);

	/* shm einhaengen */
	errno = 0;
	if ((pnShm = shmat(shmid, NULL, 0)) == (int *) -1) {
		/* FEHLERBEHANDLUNG */
		error(1, 1, "%d", errno);
		/* aufräumen */
		/* shm löschen */
		errno = 0;
		if ((shmctl(shmid,IPC_RMID,NULL)) == -1) {
			error(1, 1, "%d", errno);
		}
		/* semaphore löschen */
		if ((semrm(semid)) == -1) {
			fprintf(stderr, "semaphore error\n");
		}
		exit(EXIT_FAILURE);
	}

	/* P(semid) */
	/* daten rein schreiben */
	/* semaphore empfaenger holen */
	if ((empfaengerid = semgrep(EMPFAENGERKEY)) == -1) {
		/* FEHLERBEHANDLUNG */
		/* aufräumen */
	}
	/* V(empfaengerid) */

	/* shm aushaengen */
	if (shmdt(pnShm) == -1 ) {
		/* FEHLERBEHANDLUNG */
		error(1, 1, "%d", errno);
		/* aufräumen */
		/* shm löschen */
		errno = 0;
		if ((shmctl(shmid,IPC_RMID,NULL)) == -1) {
			error(1, 1, "%d", errno);
		}
		/* semaphore löschen */
		if ((semrm(semid)) == -1) {
			fprintf(stderr, "semaphore error\n");
		}
		exit(EXIT_FAILURE);
	}

	/* semaphore löschen */
	if ((semrm(semid)) == -1) {
		fprintf(stderr, "semaphore error\n");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
