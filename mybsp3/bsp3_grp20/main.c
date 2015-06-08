/**
 * @file main.c
 * Betriebsysteme sender / empfaenger C File.
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

#include "prog.h"
#include "bsp3lib.h"
#include "macros.h"
#include <assert.h>
#include <unistd.h>

/**
 *
 *	The make command makes two programs from this main.c file.
 *	It sets the define value EMPFAENGER or SENDER.
 *
 **/
#ifdef EMPFAENGER
	#define USAGE_MSG "\n\tusage: empfaenger -m <ringbuffer elements>"
	#define START_PROGRAM	receiver()
#else
	#ifdef SENDER
		#define USAGE_MSG "\n\tusage: sender -m <ringbuffer elements>"
		#define START_PROGRAM	sender()
	#else
		#define USAGE_MSG "\n\tThe program was compiled with wrong settings.\nPlease use 'make' to compile correctly."
		#define START_PROGRAM	ERROR	/* just for praeprocessor */
		#define COMPILE_ERROR	1
	#endif
#endif

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */
/*
 * ------------------------------------------------------------- functions --
 */

/**
 *
 * @name	main
 * 
 * @brief 	The start of sender and empfaenger
 *		
 * This is the main entry point for sender and empfaenger
 *
 * The functions starts the sender or receiver, in dependency of the const 
 * value START_PROGRAM
 *
 * @param argc the number of arguments
 * @param argv the arguments itselves (including the program name in argv[0])
 *
 * @return 0 if successfull, otherwise != 0
 *
 */

int main (int argc, char * const argv[]) 
{
	int buf_size = -1;
	const char *optstring = "m:";
	
	set_program_name(argv[0]);
    
	#ifdef COMPILE_ERROR
	/* this code cannot be executed,
	 * otherwise you have compiled the program with the wrong settings,
	 * use make to compile correctly.
	 */
	print_usage(USAGE_MSG);
	assert(0);		
	#endif
	
	/* check if command m is used. */
	if (getopt(argc, argv, optstring) != 'm')
	{
		print_usage(USAGE_MSG);
		return (EXIT_FAILURE);
	}
	
	/* check, if further arguments exists */
	if (ISNOTNULL(argv[optind]))
	{
		print_usage(USAGE_MSG);
		return (EXIT_FAILURE);
	}
	
	/* get buf_size from arguments */
	if ((buf_size = get_buffer_size(optarg)) == ERROR)
	{
		print_error("ringbuffer elements");
		print_usage(USAGE_MSG);
		return (EXIT_FAILURE);
	}
	
	/* initialize ringbuffer and semaphores */
	if (init(buf_size) == ERROR)
	{
		destroy();
		return (EXIT_FAILURE);
	}
	
	/* start the sender or receiver, in dependency of the const value START_PROGRAM */
	if (START_PROGRAM == ERROR)
	{
		destroy();
		return (EXIT_FAILURE);
	}
	
	if (fflush(NULL) < 0)
	{
		print_error("fflush error");
		return (EXIT_FAILURE);
	}
	
    return (EXIT_SUCCESS);
}


/*
 * =================================================================== eof ==
 */
