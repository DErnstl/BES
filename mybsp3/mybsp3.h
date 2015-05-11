#define SHMKEY (1000 * (int) getuid(void) + 0)
#define EMPFAENGERKEY (1000 * (int) getuid(void) + 1)
#define key (1000 * (int) getuid(void) + 2)

int mygetopts(argc, argv);
int mysemaphore(int key, int ringbuffer);
int myshmcreate(int key, int ringbuffer);
void myshmmount(int shmid, const void *shmaddr);


int mygetopts(argc, argv) {
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

        /* Ringbuffer positiv? */
        if (ringbuffer < 1) {
                fprintf(stderr, "%s: Ringbuffer must be greater 0\n", argv[0]);
                exit(EXIT_FAILURE);
        }
	return ringbuffer;
}

int mysemaphore(int key, int ringbuffer) {
        if ((senderid = seminit(key, 0600, ringbuffer)) == -1 ) {
                /* FEHLERBEHANDLUNG */
                /* -1 = semaphore existiert bereits */
                if ((senderid = semgrep(key)) == -1) {
                        /* FEHLERBEHANDLUNG */
                        fprintf(stderr, "semaphore error\n");
                        /* aufräumen nicht möglich */
                        exit(EXIT_FAILURE);
                }
        }

}

int myshmcreate(int key, int ringbuffer) {
        errno = 0;
        if ((shmid = shmget(SHMKEY, ringbuffer, 0600|IPC_CREAT|IPC_EXCL)) == -1) {
                /* FEHLERBEHANDLUNG */
                if (errno == EEXIST) {
                        if ((shmid = shmget(SHMKEY, ringbuffer)) == -1) {
                                fprintf(stderr, "shm error\n");
                                /* aufräumen */
                                if ((semrm(semid)) == -1) {
                                        fprintf(stderr, "semaphore error\n");
                                }
                                exit(EXIT_FAILURE);
                        }
                }
        }

}
