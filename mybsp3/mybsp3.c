/**
 * @file mybsp3.c
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

#include "mybsp3.h"

/*
 * -------------------------------------------------------------- variables --
 */

int semid_sender = -1;
int semid_empfaenger = -1;
int shmid = -1;
int *shmaddr = NULL;
int ringbuffer = 0;

/*
 * -------------------------------------------------------------- Functions --
 */

/**
 * 
 * \brief delete all ressources that were created by the program
 *
 * \param no parameters
 *
 * \return void, no return value, functions exits
 * 
 * \global shmid: ID of shared memory
 * \global semid_sender: ID of sender's semaphore
 * \global semid_empfaenger: ID of emppfaenger's semaphore
 * 
 */

void cleanup(void) {
	/* NULL || detach */
	if (shmid != -1) del_shm(shmid);
	if (semid_sender != -1) del_semaphore(semid_sender);
	if (semid_empfaenger != -1) del_semaphore(semid_empfaenger);
	exit(EXIT_FAILURE);
}

/**
 * 
 * \brief delete shared memory
 *
 * \param shmid: ID of shared memory
 *
 * \return errno set by shmctl
 * 
 * \global shmid: ID of shared memory
 * 
 */

int del_shm(int shmid) {
	/* shm löschen */
	errno = 0;
	if ((shmctl(shmid,IPC_RMID,NULL)) == -1) {
		error(0, 1, "%d", errno);
	}	
	return errno;
}

/**
 * 
 * \brief delete semaphore
 *
 * \param semid ID of semaphore
 *
 * \return EXIT_FAILURE in case semrm fails
 * \return EXIT_SUCCESS in case semrm does it's job
 * 
 */

int del_semaphore(int semid) {
	/* semaphore löschen */
	if ((semrm(semid)) == -1) {
		fprintf(stderr, "semaphore error\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;	
}

/**
 * 
 * \brief reads arguments given by user, checks if they are OK and returns them as int
 *
 * \param argc program name
 * \param argv size of ringbuffer
 *
 * \return EXIT_FAILURE if is of ringbuffer is smaller than 1
 * \return size of ringbuffer as int value
 * 
 */

int mygetopts(int argc, char* const argv[]) {
	int opt;
	/* parameter abfragen */
        while ((opt = getopt(argc, argv, "m:")) != -1) {
                switch (opt) {
                        case 'm':
                                ringbuffer = strtol(optarg, (char**) NULL, 10);
                                break;
                        default:
				error(1, 1, "Usage: %s <-m ringbuffer elements>\n", argv[0]);
                }
        }

        /* Ringbuffer positiv? */
        if (ringbuffer < 1) {
                fprintf(stderr, "%s: Ringbuffer must be greater 0\n", argv[0]);
                exit(EXIT_FAILURE);
        }
	return ringbuffer;
}

/**
 * 
 * \brief creates semaphore
 *
 * \param key: unique number of the semaphore
 * \param seminitvalue: initial value of semaphore
 *
 * \return senderid: id of semaphore
 * 
 * \global semid_sender: ID of sender's semaphore
 * \global semid_empfaenger: ID of emppfaenger's semaphore
 * 
 */

int mysemaphore(int key, int seminitvalue) {
	int senderid = -1;
        if ((senderid = seminit(key, ACCESSMODE, seminitvalue)) == -1 ) {
                /* -1 = semaphore existiert bereits */
                if ((senderid = semgrab(key)) == -1) {
                        /* FEHLERBEHANDLUNG */
			cleanup();
                }
        }

	if (seminitvalue == 0) {
		semid_empfaenger = senderid;
	} else {
		semid_sender = senderid;
	}

	return senderid;
}


/*
 * \brief waits for semaphore
 *
 * \param semid: ID of semaphore
 *
 * \return void, no return value
 * 
 */
void myp(int semid) {
	errno = 0;
	while (P(semid) == -1) {
		if (errno != EINTR) {
			cleanup();
		}
	}	
}

/*
 * \brief signals the semaphore
 *
 * \param semid: ID of semaphore
 *
 * \return void, no return value
 * 
 */

void myv(int semid) {
	if (V(semid) == -1) {
		cleanup();
	}
}

/**
 * 
 * \brief creates shared memory
 *
 * \param ringbuffer: size of buffer
 *
 * \return shmid: ID of shared memory
 * 
 */

int myshmcreate(int ringbuffer) {
        errno = 0;
        if ((shmid = shmget(SHMKEY, ringbuffer, ACCESSMODE|IPC_CREAT|IPC_EXCL)) == -1) {
                /* FEHLERBEHANDLUNG */
                if (errno == EEXIST) {
                        if ((shmid = shmget(SHMKEY, ringbuffer, IPC_EXCL)) == -1) {
                                /* aufräumen */
				cleanup();
                        }
                }
        }
	return shmid;
}

/**
 * 
 * \brief mounts shared memory
 *
 * \param shmid: ID of shared memory
 *
 * \return void, no return value, functions exits
 * 
 */

/* der empfänger mountet ReadOnly!
 * readonlyflag: wir verwenden hier einfach 0 für den Empfänger,
 * so wie beim Initialisieren der Semaphore */
void myshmmount(int shmid, int readonlyflag) {
        errno = 0;
	if (readonlyflag == 0) {
		if ((shmaddr = shmat(shmid, NULL, SHM_RDONLY)) == (int *) -1) {
			/* FEHLERBEHANDLUNG */
			cleanup();
		}
	}
	else {
        	if ((shmaddr = shmat(shmid, NULL, 0)) == (int *) -1) {
                	/* FEHLERBEHANDLUNG */
                	cleanup();
		}
        }
}

/**
 * 
 * \brief delete all ressources that were created by the program
 *
 * \param no parameters
 *
 * \return void, no return value, functions exits
 * 
 * \global shmaddr: address of shared memory
 */

void myshmumount(void) {
	errno = 0;
	if (shmdt(shmaddr) == -1) {
		cleanup();
	}
}
