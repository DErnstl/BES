/**
 * @file empfaenger.c
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
	
/*
 * --------------------------------------------------------------- variables --
 */

	int ringbuffer;
	char output;
	int *shmaddr_init = NULL;

	/* Parameter abfragen */
	ringbuffer = mygetopts(argc, argv);

	/* Semaphoren anlegen */
	semid_sender = mysemaphore(SENDERKEY, ringbuffer);
	semid_empfaenger = mysemaphore(EMPFAENGERKEY, 0);

	/* Shared Memory anlegen */
	shmid = myshmcreate(ringbuffer);

	/* Shared Memory einhaengen und Anfangswert merken */
	myshmmount(shmid, 1);
	shmaddr_init = shmaddr;

        /* Input lesen */
        do {
		myp(semid_empfaenger);
		output = *shmaddr;
		myv(semid_sender);
		printf("%c", output);
		shmaddr++;
		if (shmaddr_init + (ringbuffer - 1) == shmaddr) {
			shmaddr = shmaddr_init;
		}

        } while (output != EOF);

	/* Shared Memory aushaengen */
	myshmumount();

	/* Semaphore löschen */
	del_semaphore(semid_sender);

	/* Wenn der andere Prozess keine Semaphore mehr hat: aufräumen */
	if (semgrab(EMPFAENGERKEY) != -1) {
		cleanup();
	}

	return EXIT_SUCCESS;
}

