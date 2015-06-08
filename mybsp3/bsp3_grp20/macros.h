
/**
 * @file macros.h
 * Betriebsysteme marcos Header File.
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

#ifndef MACROS_H
#define MACROS_H

/*
 * -------------------------------------------------------------- includes --
 */


/*
 * --------------------------------------------------------------- defines --
 */

#define	ERROR	-1
#define SUCCESS	 0

/** 
 * @def ISNULL(pointer)		macro, which checks if the given pointer is a NULL pointer 
 */
#define ISNULL(pointer)		((pointer) == NULL)

/** 
 * @def ISNOTNULL(pointer)		macro, which checks if the given pointer isn't a NULL pointer 
 */
#define ISNOTNULL(pointer)	((pointer) != NULL)

/**
 * @def ISNULLOREMPTY(text)		macro, which checks if the given text is a NULL pointer or an empty string 
 */
#define ISNULLOREMPTY(text)		(ISNULL(text) || (*text == '\0'))


/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */

/*
 * ------------------------------------------------------------- functions --
 */



#endif

/*
 * =================================================================== eof ==
 */
