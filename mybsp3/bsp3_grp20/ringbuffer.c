/**
 * @file ringbuffer.c
 * Betriebsysteme ringbuffer C File.
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
 * --------------------------------------------------------------- defines --
 */
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "ringbuffer.h"
#include "macros.h"

/*
 * -------------------------------------------------------------- typedefs --
 */

/**
 * 
 * @struct ringbuffer_t		structure of the ringbuffer
 *
 */

typedef struct ringbuffer_t
{
	int size;
	int read;
	int write;
	int shm_id;
	int *shm_addr;

}ringbuffer_t;
/*
 * --------------------------------------------------------------- globals --
 */
static ringbuffer_t ringbuffer = {0, 0, 0, -1, NULL};
/*
 * ------------------------------------------------------------- functions --
 */

/**
 * 
 * @name get_ringbuffer_size
 *
 * @brief get's the size of the ringbuffer
 *
 * @return size of ringbuffer
 *
 */

int get_ringbuffer_size(void)
{
	return ringbuffer.size;
}

/**
 * 
 * @name set_ringbuffer_size
 *
 * @brief 		set's the ringbuffer size
 *
 * @param size		size of the ringbuffer
 *
 * @return 		error if size <= 0 - otherwise success
 *
 */

int set_ringbuffer_size(int size)
{
	if (size <= 0)
 	{ 
 		errno = EINVAL; 
 		return ERROR;
 	}
	
	ringbuffer.size = size;
	return SUCCESS;
}

/**
 * 
 * @name create_ringbuffer
 *
 * @brief 	creates and attaches the shared memory for the ringbuffer
 *
 * @param	size - the size of the ringbuffer
 * @param	key  - the unique key of the shared memory
 *
 * @return 	success, or otherwise error if creating fails
 *
 */

int create_ringbuffer(int size,key_t key)
{
	if(key <= 0 || size <= 0)
	{
		errno = EINVAL;
		return ERROR;
	}
   
	/* get shared memory, or if not exist create shared memory */
	if((ringbuffer.shm_id = shmget(key, sizeof(int) * size, 0660 | IPC_CREAT)) == ERROR)
	{
		return ERROR;
	}

	/* attach shared memory */
	if((ringbuffer.shm_addr = shmat(ringbuffer.shm_id, NULL, 0)) == (int *)-1)
	{
		return ERROR;
	}

	/* initialize ringbuffer */
	set_ringbuffer_size(size);
	ringbuffer.read = 0;
	ringbuffer.write = 0;

    return SUCCESS;
}

/**
 * 
 * @name 	free_ringbuffer
 *
 * @brief 	detaches the shared memory
 *
 * @return 	success, or otherwise error if detaching fails
 *
 */

int free_ringbuffer(void)
{
	if (shmdt(ringbuffer.shm_addr) == ERROR)
    {
		return ERROR;
    }
	
	return SUCCESS;
}


/**
 * 
 * @name 	destroy_ringbuffer
 *
 * @brief 	detaches and removes the shared memory
 *
 * @return 	success, or otherwise error if detaching or removing fails
 *
 */

int destroy_ringbuffer(void)
{
	/* if no address and no id exist, nothing to do. */
	if (ISNULL(ringbuffer.shm_addr) && ringbuffer.shm_id < 0)
	{
		return SUCCESS;
	}
	
    if(ISNULL(ringbuffer.shm_addr) || ringbuffer.shm_id < 0)
    {
        errno = EINVAL;
        return ERROR;
    }

	/* detach shared memory */
    if (free_ringbuffer() == ERROR)
	{
		return ERROR;
	}

	/* remove shared memory */
    if (shmctl(ringbuffer.shm_id, IPC_RMID, NULL) == ERROR)
    {
        return ERROR;
    }

	/* reset ringbuffer values */
	ringbuffer.size = 0;
	ringbuffer.read = 0;
	ringbuffer.write = 0;
	ringbuffer.shm_addr = NULL;
	ringbuffer.shm_id = -1;
	
	return SUCCESS;
}


/**
 * 
 * @name 	write_next_element
 *
 * @brief 	writes the element to the ringbuffer
 *		writing the element is working without synchronisation
 *
 * @return 	success, or error if shared memory is not allocated
 *
 */

int write_next_element(int element)
{
	if(ISNULL(ringbuffer.shm_addr))
	{
		errno = EBADF;
		return ERROR;
	}
	
	ringbuffer.write %= ringbuffer.size;
	ringbuffer.shm_addr[ringbuffer.write] = element;
	ringbuffer.write++;
    
    return SUCCESS;
}

/**
 * 
 * @name 	read_next_element
 *
 * @brief 	reads the next element from the ringbuffer
 *		reading the element is working without synchronisation
 *
 * @return 	success, or error if shared memory is not allocated
 *
 */

int read_next_element(void)
{
	int result;
	
	if(ISNULL(ringbuffer.shm_addr))
	{
		errno = EBADF;
		return ERROR;
	}
	
	ringbuffer.read %= ringbuffer.size;
	result = ringbuffer.shm_addr[ringbuffer.read];
	ringbuffer.read++;
	
	return result;

}

/*
 * =================================================================== eof ==
 */
