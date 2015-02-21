/**
 * @file myFind.c
 * Betriebssysteme myFind File
 * Beispiel 1
 *
 * @author Adam Kerenyi <ic14b080@technikum-wien.at>
 * @author Romeo Beck <ic14b037@technikum-wien.at>
 * @author Thomas Zeitinger <ic14b033@technikum-wien.at>
 * @date 2015/02/09
 *
 * @version 470
 *
 * @todo Test it more seriously and more complete.
 * @todo Review it for missing error checks.
 * @todo Review it and check the source against the rules at
 *       https://cis.technikum-wien.at/documents/bic/2/bes/semesterplan/lu/c-rules.html
 *
 */

/*
 * -------------------------------------------------------------- includes --
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <errno.h>
#include <string.h>
#include <fnmatch.h>
#include <string.h>
#include <pwd.h>		/* getpwnam("username") || getpwuid(uid_t uid) */
#include <sys/stat.h>	/* lstat() -> -print(), -ls() */
#include <dirent.h>		/* DIR *opendir(const char* dirname) //full of relative path */


/*
 * --------------------------------------------------------------- defines --
 */

/* Contant Definitions */
#define PRINT	0
#define LS		1 
#define USER	2
#define NOUSER	3
#define PATH	4
#define NAME	5
#define TYPE	6
#define MAXNAMELENGHT 255

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */

extern int errno;

/*
 * ------------------------------------------------------------- functions --
 */


/* Function Prototypes */
int usage (void);
void do_file(const char * file_name, const char * const * parms);	/* return type void as none was given */
void do_dir(const char * dir_name, const char * const * parms);		/* return type void as none was given */
int check_parameters();	/* MISSING: add parameters */
int prepare_path();	/* MISSING: add parameters */
int check(char file_name, const char * const * parms, int position);

/*TODO: Romeo */
int check_user(/*TODO*/);
int check_nouser(/*TODO*/);

/*TODO: Adam */
int check_type(/*TODO*/);
int print_ls(/*TODO*/);
int print(/*TODO*/);

/*TODO: Tom */
int check_name(char file_name, const char * const * parms, int position /*TODO*/) {
int check_path(char file_name, const char * const * parms, int position /*TODO*/) {


/**
 *
 * \brief The most unusful, but educational Linux program ;-)
 *
 * This is the main entry point for any C program.
 *
 * \param argc the number of arguments
 * \param argv the arguments itselves (including the program name in argv[0])
 *
 * \return always "success"
 * \retval 0 always
 *
 */


/* Main Function */
int main(int argc, char *argv[]) {

	/* We want argv as constant */
	const char * const *paramlist  = (const char * const *)&argv[0];

	/* Declare variables */
	char * path;

	/* Check if parameters are correct */
	check_parameters(/*TODO*/);

	/* Check and prepare path if necessary */
	prepare_path(/*TODO*/);

	/* Go through all files */
	do_dir(/*TODO*/);

	exit(EXIT_SUCCESS);
}


/* Function Definitions */
void check_parameters() {
	/* NOTE: Consider what errors to check for */
}

void prepare_path(const char * path) {
	/* Check if path argv[1] is set and if it is a directory */
}


void do_file(const char * file_name, const char * const * parms) {
	int position = 0;
	/* create filedescriptor */
	struct stat fd_in;

	/* lstat file and write content to struct 
	 * throw error if failed with Filename */
	if (lstat(file_name, &fd_in) == -1) {
		perror(parms[0]);
		exit(EXIT_FAILURE);
	}

	/* TODO */
	check(file_name, parms, position);
}

void do_dir(const char * dir_name, const char * const * parms) {
	/* Create DIR Struct */
	DIR *dir;
	struct dirent *d;

	/* opendir an throw error with exename if error */
	if (dir = opendir(dir_name) == NULL ) {
			perror(parms[0]);
			exit(EXIT_FAILURE);
	}

	/* readdir until NULL */
	while ((d = readdir(dir)) != NULL ) {
		/* TODO */
		do_file((*d).d_name,parms);
	}

	closedir(dir_name);
}

int check(char file_name, const char * const * parms, int position) {
	switch(parms[position]) {
			case NAME:	( (check_name(/*TODO*/)) && position=position+2 ) && ( (check(file_name,parms,position)) || print(/*TODO*/) ); break;
			case PATH:	( (check_path(/*TODO*/)) && position=position+2 ) && ( (check(file_name,parms,position)) || print(/*TODO*/) ); break;
			case USER:	( (check_user(/*TODO*/)) && position=position+2 ) && ( (check(file_name,parms,position)) || print(/*TODO*/) ); break;
			case NOUSER:	( (check_nouser(/*TODO*/)) && position++ ) && ( (check(file_name,parms,position)) || print(/*TODO*/) ); break;
			case TYPE:	( (check_type(/*TODO*/)) && position=position+2 ) && ( (check(file_name,parms,position)) || print(/*TODO*/) ); break;
			case LS:	( (print_ls(/*TODO*/)) && position=position+2 ) && (check(file_name,parms,position)); break;
			case PRINT:	( (print(/*TODO*/)) && position=position+2 ) && (check(file_name,parms,position)); break;
			default: return EXIT_FAILURE;

	}
}

int check_name(char file_name, const char * const * parms, int position /*TODO*/) {

	if(fnmatch(parms[position+1],file_name,FNM_NOESCAPE) == 0) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}

}

int check_path(char file_name, const char * const * parms, int position /*TODO*/) {

	if(fnmatch(parms[position+1],file_name,FNM_NOESCAPE) == 0) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}

}

/*
 * =================================================================== eof ==
 */

