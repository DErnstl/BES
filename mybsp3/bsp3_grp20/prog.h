
/**
 * @file prog.h
 * Betriebsysteme prog Header File.
 *
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

#ifndef PROG_H
#define PROG_H

/*
 * -------------------------------------------------------------- includes --
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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

const char *get_program_name(void);

void set_program_name(const char *prog_name);

void print_usage(const char *usage);

void print_error(const char *message);

#endif

/*
 * =================================================================== eof ==
 */
