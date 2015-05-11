#define SHMKEY (1000 * (int) getuid(void) + 0)
#define EMPFAENGERKEY (1000 * (int) getuid(void) + 1)
#define SENDERKEY (1000 * (int) getuid(void) + 2)
#define MODE 0600

static int semid_sender = -1;
static int semid_empfaenger = -1;
static int shmid = -1;
static int *shmaddr = NULL;

int mygetopts(int argc, const char *argv[]);
int del_shm(int shmid);
int del_semaphore(int semid);
void cleanup(void);
int mysemaphore(int key, int ringbuffer);
int myshmcreate(int key, int ringbuffer);
void myshmmount(int shmid, const void *shmaddr);


void cleanup(void) {
	/* NULL || detach */
	if (shmid != -1) del_shm(shmid);
	if (semid_sender != -1) del_semaphore(semid_sender);
	if (semid_empfaenger != -1) del_semaphore(semid_empfaenger);
	exit(EXIT_FAILURE);
}

int del_shm(int shmid) {
	/* shm löschen */
	errno = 0;
	if ((shmctl(shmid,IPC_RMID,NULL)) == -1) {
		error(0, 1, "%d", errno);
	}	
	return errno;
}

int del_semaphore(int semid) {
	/* semaphore löschen */
	if ((semrm(semid)) == -1) {
		fprintf(stderr, "semaphore error\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;	
}

int mygetopts(argc, argv) {
        /* parameter abfragen */
        while ((opt = getopt(argc, argv, "m:")) != -1) {
                switch (opt) {
                        case 'm':
                                ringbuffer = atoi(optarg);
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

int mysemaphore(int key, int seminitvalue) {
        if ((senderid = seminit(key, MODE, seminitvalue)) == -1 ) {
                /* FEHLERBEHANDLUNG */
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

int myshmcreate(int key, int ringbuffer) {
        errno = 0;
        if ((shmid = shmget(SHMKEY, ringbuffer, MODE|IPC_CREAT|IPC_EXCL)) == -1) {
                /* FEHLERBEHANDLUNG */
                if (errno == EEXIST) {
                        if ((shmid = shmget(SHMKEY, ringbuffer)) == -1) {
                                /* aufräumen */
				cleanup();
                        }
                }
        }
	return shmid;
}

(int *) myshmmount(int shmid, const void *shmaddr) {
        errno = 0;
        if ((shmaddr = shmat(shmid, NULL, 0)) == (int *) -1) {
                /* FEHLERBEHANDLUNG */
                cleanup();
        }
	return shmaddr;
}
