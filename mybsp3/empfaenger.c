#ifndef MYBSP3.H
#include "mybsp3.h"
#endif

/* Ich bin der Sender */

/* TODO:
 * - P
 * - daten rein schreiben
 * - V
 * - Signalbehandlung */

int main(int argc, const char *argv[]) {

	int ringbuffer;
	char output[1];
	const int *shmaddr_init = NULL;

	/* parameter abfragen */
	ringbuffer = mygetopts(argc, argv);

	/* semaphore anlegen */
	semid_sender = mysemaphore(SENDERKEY, ringbuffer);
	semid_empfaenger = mysemaphore(EMPFAENGERKEY, 0);

	/* shm anlegen */
	shmid = myshmcreate(SHMKEY, ringbuffer);

	/* shm einhaengen */
	shmaddr = myshmmount(shmid, 1);
	shmaddr_init = shmaddr;

	/* P(semid) */
        /* Process the read of the input */
        do {
		myp(semid_empfaenger);
		output = *shmaddr;
		myv(semid_sender);
		fprintf("%c", output);
		&shmaddr++;
		if (&shmaddr_init + (ringbuffer - 1) == &shmaddr) {
			&shmaddr = &shmaddr_init;
		}

        } while (input != EOF);

	/* daten rein schreiben (Modulo) */
	/* semaphore empfaenger holen */
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

