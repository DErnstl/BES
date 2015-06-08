
/**
 * @file bsp3lib.h
 * Betriebsysteme Beispiel3 libaries Header File.
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

#ifndef BSP3LIB_H
#define BSP3LIB_H

/*
 * -------------------------------------------------------------- includes --
 */


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

int get_buffer_size(const char *argv);
int init(int buf_size);
int destroy(void);

int receiver(void);
int sender(void);

#endif

/*
 * =================================================================== eof ==
 */
