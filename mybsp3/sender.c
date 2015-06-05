/**
 * @file sender.c
 * Betriebssysteme myfind
 * Beispiel 3
 *
 * @author Adam Kerenyi <ic14b080@technikum-wien.at>
 * @author Romeo Beck <ic14b037@technikum-wien.at>
 * @author Thomas Zeitinger <ic14b033@technikum-wien.at>
 * @date 2015/02/09
 *
 * @version 100
 *
 */

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

