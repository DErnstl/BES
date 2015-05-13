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
 *
 * brauchen wir ein struct shmid_ds (man smhctl und man shmat)?
 * shmmount ist für den empfänger read-only!
 * - P
 * - daten rein schreiben und merken wo wir im buffer beim lesen/schreiben sind
 * - V
 * - Signalbehandlung */

int main(int argc, const char *argv[]) {

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
	myshmumount();

	/* semaphore löschen */
	del_semaphore(semid_sender);

	/* wenn der andere Prozess keine Semaphore mehr hat: aufräumen */
	if (semgrab(EMPFAENGERKEY) != -1) {
		cleanup();
	}

	return EXIT_SUCCESS;
}

