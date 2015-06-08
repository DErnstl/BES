/**
 * @file sender.c
 * Betriebssysteme myfind
 * Beispiel 3
 *
 * @author Adam Kerenyi <ic14b080@technikum-wien.at>
 * @author Romeo Beck <ic14b037@technikum-wien.at>
 * @author Thomas Zeitinger <ic14b033@technikum-wien.at>
 * @date 2015/06/05
 *
 * @version 100
 *
 */
 
 /*
 * -------------------------------------------------------------- includes --
 */

#ifndef MYBSP3_H
#include "mybsp3.h"
#endif

/*
 * -------------------------------------------------------------------- main--
 */

int main(int argc, char* const argv[]) {
	int ringbuffer;
	int input;
	int *shmaddr_init = NULL;

	/* Parameter abfragen */
	ringbuffer = mygetopts(argc, argv);

	/* Semaphore anlegen */
	semid_sender = mysemaphore(SENDERKEY, ringbuffer);
	semid_empfaenger = mysemaphore(EMPFAENGERKEY, 0);

	/* Shared Memory anlegen */
	shmid = myshmcreate(ringbuffer);

	/* Shared Memory einhaengen */
	myshmmount(shmid, 1);
	shmaddr_init = shmaddr;

        /* Process send */
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

	/* Shared Memory aushaengen */
	myshmumount();

	return EXIT_SUCCESS;
}

