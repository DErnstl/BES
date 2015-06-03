#ifndef MYBSP3_H
#include "mybsp3.h"
#endif

/* Ich bin der Sender */

int main(int argc, char* const argv[]) {
	int ringbuffer;
	int input;
	int *shmaddr_init = NULL;

	/* parameter abfragen */
	ringbuffer = mygetopts(argc, argv);

	/* semaphore anlegen */
	semid_sender = mysemaphore(SENDERKEY, ringbuffer);
	semid_empfaenger = mysemaphore(EMPFAENGERKEY, 0);

	/* shm anlegen */
	shmid = myshmcreate(ringbuffer);

	/* shm einhaengen */
	myshmmount(shmid, 1);
	shmaddr_init = shmaddr;

	/* P(semid) */
        /* Process the read of the input */
        do {
                input = fgetc(stdin);
		myp(semid_sender);
		*shmaddr = input;
		myv(semid_empfaenger);
		shmaddr++;
		if (shmaddr_init + (ringbuffer - 1) == shmaddr) {
			shmaddr = shmaddr_init;
		}

        } while (input != EOF);

	/* daten rein schreiben (Modulo) */
	/* semaphore empfaenger holen */
	/* V(empfaengerid) */

	/* shm aushaengen */
	myshmumount();

	return EXIT_SUCCESS;
}

