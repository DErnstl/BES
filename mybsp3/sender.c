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

/* offene Fragen:
 *
 * wie finde ich die semaphore des anderen prozesses?
 * soll ich die keys für jeden prozess statisch als DEFINE verwenden?
 *
 */

static int semid_sender;
static int semid_empfaenger;

#define SHMKEY (1000 * (int) getuid(void) + 0)
#define EMPFAENGERKEY (1000 * (int) getuid(void) + 1)
#define SENDERKEY (1000 * (int) getuid(void) + 2)

/*int getkey(int role) {
	int keybase = 1000 * (int) getuid(void);

	if (role < 1 || role > 2) {
		fprintf(stderr, "Undefined Role, please check source. I die.\n");
		exit(EXIT_FAILURE);
	}

	return keybase + role;
}*/

int main(int argc, const char *argv[]) {

	int opt;
	int ringbuffer;
	int role = 1; /* sender = 1, empfaenger = 2 */
	int mykey;
	int semid;
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

	/* key anlegen */
	mykey = getkey(role);
	/* semaphore anlegen */
	if ((semid = seminit(mykey, 0660, ringbuffer)) == -1 ) {
		/* FEHLERBEHANDLUNG */
		/* -1 = semaphore existiert bereits */
	}

	/* shm anlegen */
	if ((shmid = shmget(mykey, ringbuffer, 0660|IPC_CREAT)) == -1) {
		/* FEHLERBEHANDLUNG */
	}

	/* shm einhaengen */
	if ((pnShm = shmat(shmid, NULL, 0)) == (int *) -1) {
		/* FEHLERBEHANDLUNG */
	}

	/* P */
	/* daten rein schreiben */
	/* V */

	/* shm aushaengen */
	if (shmdt(pnShm) == -1 ) {
		/* FEHLERBEHANDLUNG */
	}

	return EXIT_SUCCESS;
}
