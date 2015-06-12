/**
 * @file bsp3lib.c
 * Betriebsysteme Beispiel3 libaries C File.
 *
 * @author Stammgruppe 20
 * @author Zuebide Sayici 	ic14b002
 * @author Kurt Liebenberg 	ic14b062
 * @author Markus Wals		ic14b076
 * @author Manfred Lackner  ic13b071
 *
 * @date 2015/06/05
 *
 * URL: $HeadURL$
 *
 * Last Modified: $Author: Lackner $
 */

/*
 * -------------------------------------------------------------- includes --
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <assert.h>
#include <sem182.h>
#include <signal.h>
#include "bsp3lib.h"
#include "macros.h"
#include "prog.h"
#include "ringbuffer.h"

/*
 * --------------------------------------------------------------- defines --
 */

/* ### FB: laut Angabe sollten wir die Keys dynamisch generieren */
#define RINGBUF_KEY	10258006
#define SEM_READ_KEY	10258053
#define SEM_WRITE_KEY	10258013

#define	SEM_PERM	0660

/*
 * -------------------------------------------------------------- typedefs --
 */

/**
 * 
 * @struct the semaphor datatype 
 *
 */

typedef struct syncsem
{
	int sem_read;
	int sem_write;
} syncsem_t;
/*
 * --------------------------------------------------------------- globals --
 */
static syncsem_t syncsem = {-1, -1};

/*
 * ------------------------------------------------------------- functions --
 */

static int create_sem(key_t sem_key, int pos);
static int destroy_sem(int semid);
static int up(int semid);
static int down(int semid);
static void terminate(int mysignal);

/* SEM LIB BEGIN */

/**
 * 
 * @name create_sem 
 *
 * @brief  is allocating the semaphor memory
 *
 * @param       sem_key contains the semaphor access key
 * @param       pos     actual position
 *
 * @return      on error -1 or sem_id on success
 *              
 */
static int create_sem(key_t sem_key, int pos)
{
	int sem_id = -1;
	
	if (sem_key <= 0)
	{
		errno = EINVAL;
		return ERROR;
	}
	
	/* create new semaphore */
	if ((sem_id = seminit(sem_key, SEM_PERM, pos)) == ERROR)
	{
		/* if semaphore still exists, grab the semaphore */
		if ((sem_id = semgrab(sem_key)) == ERROR)
		{
			/* ### FB: Warum ENOMEM? Koennte ja z.B. schon eine mit dem selben Key, aber anderen
			 * 	Zugriffsrechten angelegt sein */
			errno = ENOMEM;
			return ERROR;
		}
		
		errno = 0;
	}
	
	return sem_id;
}

/**
 * 
 * @name destroy_sem 
 *
 * @brief destroying the semaphor
 *
 * @param no param
 * 
 * @return on error -1 or 0 on success
 * 
 */
static int destroy_sem(int semid)
{
	if (semid < 0)
	{
		errno = EINVAL;
		return ERROR;
	}
	
	/* remove semaphore */
	if (semrm(semid) == ERROR)
	{
		if (errno == EINVAL)
		{
			/* if semid doesn't exist (anymore), nothing to do. */
			return SUCCESS;
		}
		
		return ERROR;
	}
	
	return SUCCESS;
}

/**
 * 
 * @name up
 *
 * @brief increasing the semaphor
 *
 * @param sem_id        contains the Semaphor_ID
 * 
 * @return      on error -1 or 0 on success
 * 
 */
static int up(int semid)
{
	while (V(semid) == ERROR)
	{
		if (errno != EINTR)
		{
			return ERROR;
		}
	}
	
	return SUCCESS;
}

/**
 * 
 * @name down
 *
 * @brief decreasing the semaphor
 *
 * @param sem_id        contains the Semaphor_ID
 * 
 * @return      on error -1 or 0 on success
 * 
 */
static int down(int semid)
{
	while (P(semid) == ERROR)
	{
		if (errno != EINTR)
		{
			return ERROR;
		}
	}
	
	return SUCCESS;
}

/* SEM LIB END */

/**
 * 
 * @name init
 *
 * @brief initializes the semaphor and the shared memory
 *
 * @param buf_size contains the buffer size.
 * 
 * @return      on error -1 or 0 on success
 * 
 */
int init(int buf_size)
{
	if (create_ringbuffer(buf_size, RINGBUF_KEY) == ERROR)
	{
		print_error("Creating Ringbuffer");
		return ERROR;
	}
	
	if ((syncsem.sem_read = create_sem(SEM_READ_KEY, 0)) == ERROR)
	{
		print_error("Creating Semaphore");
		return ERROR;
	}
	
	if ((syncsem.sem_write = create_sem(SEM_WRITE_KEY, buf_size)) == ERROR)
	{
		print_error("Creating Semaphore");
		return ERROR;
	}
	
	if (signal(SIGINT, terminate) == SIG_ERR)
	{
		print_error("Creating Signal");
		return ERROR;
	}
	
	return SUCCESS;
}

/**
 * 
 * @name destroy
 *
 * @brief is removing the semaphor and the shared memory from the system
 *
 * @param no_params
 * 
 * @return      on error -1 or 0 on success
 * 
 */
int destroy(void)
{
	if (destroy_ringbuffer() == ERROR)
	{
		print_error("Destroying ringbuffer.");
		return ERROR;	
	}
	 
	if (destroy_sem(syncsem.sem_read) == ERROR)
	{
		print_error("Destroying Semaphore");
		return ERROR;
	}
	
	syncsem.sem_read = -1;
	
	if (destroy_sem(syncsem.sem_write) == ERROR)
	{
		print_error("Destroying Semaphore");
		return ERROR;
	}
	
	syncsem.sem_write = -1;
	
	return SUCCESS;
	
}

/**
 * 
 * @name terminate
 *
 * @brief terminates the process
 *
 * @param mysignal	contains kill signal
 *
 */

static void terminate(int mysignal)
{
	if(mysignal == SIGINT)
	{
		destroy();
		exit(EXIT_FAILURE);
	}
}

/**
 * 
 * @name get_buffer_size
 * 
 * @brief set's the buffer size to the given size
 *
 * @param argv contains the given arguments(buff_size)
 *
 */

int get_buffer_size(const char *argv)
{
	int buf_size = 0;
	char *ende = NULL;

	/* ### FB: ISNULLOREMPTY beinhaltet ISNULL, deshalb wird das doppelt abgefragt */
	if (ISNULL(argv) || ISNULLOREMPTY(argv))
	{
		errno = EINVAL;
		return ERROR;
	}

	/* ### FB: in der Angabe steht zwar nicht explizit, dass wir Dezimalzahlen verwenden sollen,
	 * 	aber ob von Euch geplant war, dass man mit "021" z.B. Octalwerte übergibt, weiß ich nicht :) */
	buf_size = strtol(argv, &ende, 0);
		
	if (errno == ERANGE)
	{
		return ERROR;
	}
	
	/* ### FB: ich verstehe nicht was Ihr hier tun wolltet, aber die Schleife wird nie ausgefuehrt:
	 * 	ISNULL(text) -> (pointer) == NULL -> NULL == NULL -> wahr
	 * 	(*text == '\0') -> NULL == '\0' -> wahr
	 * 	-> wahr || wahr -> wahr und dann die Verneinung -> falsch */
	if (!ISNULLOREMPTY(ende))
	{
		errno = EINVAL;
		return ERROR;
	}
	
	if (buf_size <= 0)
	{
		errno = EDOM;
		return ERROR;
	}

	return buf_size;

}

/**
 * 
 * @name sender
 *
 * @brief sender of the message
 *
 * @param no param
 * @return      error = -1
 *                      success = 0
 */
int sender(void)
{	
	int input;
	int result;
	
	do
	{
		if ((input = fgetc(stdin)) < 0)
		{
			if (ferror(stdin))
			{
				print_error("fgetc");
				return ERROR;
			}
			/* ### FB: Gut! */
			if (feof(stdin))
			{
				/* make sure, that the value is really EOF. */
				input = EOF;
			}
		}
		
		if (down(syncsem.sem_write) == ERROR)
		{
			print_error("P(sem_write)");
			return ERROR;
		}
		
		/* write the characters into the ringbuffer.
		 * EOF signals the ending to the reader of the ringbuffer
		 */
		if((result = write_next_element(input)) == ERROR)
		{
			if (errno == EBADF)
			{
				print_error("SHM write");
				return ERROR;
			}
		}
		
		if (up(syncsem.sem_read) == ERROR)
		{
			print_error("V(sem_read)");
			return ERROR;
		}
		
	}
	while (input != EOF);
	
	/* sending completed, deteach the ringbuffer */
	if (free_ringbuffer() == ERROR)
	{
		print_error("Could not deteach ringbuffer.");
		return ERROR;	
	}
	
	return SUCCESS;
}

/**
 * 
 * @name receiver
 *
 * @brief receiver of the message
 *
 * @param no param
 * @return      error = -1
 *              success = 0
 */
int receiver(void)
{
	int output;
	
	do 
	{
		if (down(syncsem.sem_read) == ERROR)
		{
			
			print_error("P(sem_read)");
			return ERROR;
		}
		
		/* read character from the ringbuffer */
		output = read_next_element();
	
		if (errno == EBADF)
		{
			print_error("SHM read");
			return ERROR;
		}
		
		if (up(syncsem.sem_write) == ERROR)
		{
			print_error("V(sem_write)");
			return ERROR;
		}
		
		if (output != EOF)
		{
			/* not EOF, then write character to stdout */
			if (fputc(output, stdout) < 0)
			{
				if (ferror(stdout))
				{
					print_error("fputc");
					return ERROR;
				}
			}
		}
		 
	} 
	while (output != EOF);

	if (fflush(stdout) < 0)
	{
		print_error("fflush");
		return ERROR;
	}
	
	/* receiving completed, destroy ringbuffer and semaphores. */
	destroy();

	return SUCCESS;
}

/*
 * =================================================================== eof ==
 */
