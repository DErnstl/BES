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
	int semid_sender;
	int semid_empfaenger;
	int shmid;
	int *pnShm = NULL;

	/* parameter abfragen */
	ringbuffer = mygetopts(argc, argv);

	/* semaphore anlegen */
	semid_sender = mysemaphore(SENDERKEY, ringbuffer);
	semid_empfaenger = mysemaphore(EMPFAENGERKEY, 0);

	/* shm anlegen */
	shmid = myshmcreate(SHMKEY, ringbuffer);

	/* shm einhaengen */
	pnShm = myshmmount(shmid);

	/* P(semid) */
	/* daten rein schreiben (Modulo) */
	/* semaphore empfaenger holen */
	if ((empfaengerid = semgrep(EMPFAENGERKEY)) == -1) {
		/* FEHLERBEHANDLUNG */
		/* aufräumen */
	}
	/* V(empfaengerid) */

	/* shm aushaengen */
	if (shmdt(pnShm) == -1 ) {
		/* FEHLERBEHANDLUNG */
		error(0, 1, "%d", errno);
		/* aufräumen */
		/* shm löschen */
		errno = 0;
		if ((shmctl(shmid,IPC_RMID,NULL)) == -1) {
			error(0, 1, "%d", errno);
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
