
/**
 * @file ringbuffer.h
 * Betriebsysteme prog Header File.
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

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

/*
 * -------------------------------------------------------------- includes --
 */
#include <sys/types.h>


/*
 * --------------------------------------------------------------- defines --
 */

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */

/*
 * ------------------------------------------------------------- functions --
 */
int get_ringbuffer_size(void);
int set_ringbuffer_size(int size);
int create_ringbuffer(int size,key_t key);

int free_ringbuffer(void);
int destroy_ringbuffer(void);
int write_next_element(int element);
int read_next_element(void);


#endif

/*
 * =================================================================== eof ==
 */
