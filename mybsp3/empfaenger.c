##
## @file empfaenger
## Betriebssysteme mybsp3 Makefile
## Beispiel 3
##
## @author Adam Kerenyi <ic14b080@technikum-wien.at>
## @author Romeo Beck <ic14b037@technikum-wien.at>
## @author Thomas Zeitinger <ic14b033@technikum-wien.at>
## @date 2015/06/05
##
## @version $Revision: 972 $
##
##

#ifndef MYBSP3_H
#include "mybsp3.h"
#endif

int main(int argc, char* const argv[]) {

	int ringbuffer;
	char output;
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
		myp(semid_empfaenger);
		output = *shmaddr;
		myv(semid_sender);
		printf("%c", output);
		shmaddr++;
		if (shmaddr_init + (ringbuffer - 1) == shmaddr) {
			shmaddr = shmaddr_init;
		}

        } while (output != EOF);

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

