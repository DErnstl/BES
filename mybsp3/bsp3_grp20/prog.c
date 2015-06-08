/**
 * @file prog.c
 * Betriebsysteme prog C File.
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
 * --------------------------------------------------------------- includes --
 */
#include "prog.h"
#include "macros.h"

/*
 * --------------------------------------------------------------- defines --
 */

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */
static const char *progname = NULL;	/* program name, for error handling */
/*
 * ------------------------------------------------------------- functions --
 */

/**
 * 
 * @name get_program_name
 *
 * @brief get's the program name from the const char *progname
 *
 * @return progname	string of the program name.
 *
 */
const char *get_program_name(void)
{
	return progname;
}

/**
 * 
 * @name set_program_name
 *
 * @brief set's the program name in the const char *progname
 *
 * @param prog_name	string of the program name.
 *
 */
void set_program_name(const char *prog_name)
{
	if (ISNOTNULL(prog_name))
		progname = prog_name;
}

/**
 * 
 * @name print_usage
 *
 * @brief prints the usage message
 *
 * The function show_usage prints the usage_msg. 
 *
 * @param usage_msg text of the usage message
 *
 */	
void print_usage(const char * usage_msg)
{
	if (fprintf(stdout, "%s: %s\n",progname, usage_msg) < 0)
	{
		print_error("fprintf error");
		exit(EXIT_FAILURE);
	}
}

/**
 * 
 * @name print_error
 *
 * @brief prints an error message
 *
 * @param message	additional information
 *
 */
void print_error(const char * message)
{
	/* ##' FB: nur als Tipp: error() bringt den Programnamen schon mit und so
	 * 	ist keine eigene Variable notwendig */
	if (fprintf(stderr, "%s: %s: %s\n", progname, message, strerror(errno)) < 0)
	{
		exit(EXIT_FAILURE);
	}
}
/*
 * =================================================================== eof ==
 */
