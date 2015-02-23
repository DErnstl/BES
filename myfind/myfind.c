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
#define MAXNAMELENGHT 255

#define NAME (strcmp(*parms[argv_pos], "-name") == NULL)
#define PATH (strcmp(*parms[argv_pos], "-path") == NULL)
#define USER (strcmp(*parms[argv_pos], "-user") == NULL)
#define NOUSER (strcmp(*parms[argv_pos], "-nouser") == NULL)
#define TYPE (strcmp(*parms[argv_pos], "-type") == NULL)
#define LS (strcmp(*parms[argv_pos], "-ls") == NULL)
#define PRINT (strcmp(*parms[argv_pos], "-print") == NULL)

#define CHECK (check(fd_in,&parms,argv_pos))

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
void do_file(const char * file_name, const char * const * parms);	/* return type void as none was given */
void do_dir(const char * dir_name, const char * const * parms);		/* return type void as none was given */
int check(struct stat file, const char * const * parms, int argv_pos);

/*TODO: Romeo */
int check_user(/*TODO*/);
int check_nouser(/*TODO*/);
int check_parameters();	/* MISSING: add parameters */

/*TODO: Adam */
int check_type(/*TODO*/);
int print_ls(/*TODO*/);
int print(/*TODO*/);
int usage (void); /*Adam*/

/*TODO: Tom */
int check_name(char file_name, const char * const * parms, int argv_pos);
int check_path(char file_name, const char * const * parms, int argv_pos);
/* Check-FUnktion und Rekursion für dodir*/


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
int main(int argc, const char *argv[]) {

	/* Declare variables, need as const array of all parameters */
	const char * const *paramlist = (const char * const *)&argv[0];
	const char *filename = (const char *)&argv[1];

	/* Check if parameters are correct */
	check_parameters(/*TODO*/);

	/* Go through all files */
	do_dir(filename,paramlist);

	exit(EXIT_SUCCESS);
}


/* Function Definitions */
int check_parameters() {
	/* NOTE: Consider what errors to check for */
}


void do_file(const char * file_name, const char * const * parms) {
	int argv_pos = 2;
	/* create filedescriptor */
	struct stat fd_in;

	/* lstat file and write content to struct 
	 * throw error if failed with Filename */
	if (lstat(file_name, &fd_in) == -1) {
		perror(parms[0]);
		exit(EXIT_FAILURE);
	}

	/* TODO */
	check(fd_in, parms, argv_pos);
	if (S_ISDIR(fd_in.st_mode)) { do_dir(file_name,parms); }
}

void do_dir(const char * dir_name, const char * const * parms) {
	/* Create DIR Struct */
	DIR *dir;
	struct dirent *d;

	/* opendir an throw error with exename if error */
	if ((dir = opendir(dir_name)) == NULL ) {
			perror(parms[0]);
			exit(EXIT_FAILURE);
	}

	/* readdir until NULL */
	while ((d = readdir(dir)) != NULL ) {
		/* TODO: . und .. weg*/
		do_file((*d).d_name,parms);
	}

	closedir(dir);
}

int check(struct stat file, const char * const * parms, int argv_pos) {
	if NAME {( (check_name(/*TODO*/)) && argv_pos=argv_pos+2 ) && ( CHECK || print(/*TODO*/) ); }
	else if PATH {( (check_path(/*TODO*/)) && argv_pos=argv_pos+2 ) && ( CHECK || print(/*TODO*/) ); }
	else if USER {( (check_user(/*TODO*/)) && argv_pos=argv_pos+2 ) && ( CHECK || print(/*TODO*/) ); }
	else if NOUSER {( (check_nouser(/*TODO*/)) && argv_pos++ ) && ( CHECK || print(/*TODO*/) ); }
	else if TYPE {( (check_type(/*TODO*/)) && argv_pos=argv_pos+2 ) && ( CHECK || print(/*TODO*/) ); }
	else if LS {( (print_ls(/*TODO*/)) && argv_pos=argv_pos+2 ) && CHECK; }
	else if PRINT {( (print(/*TODO*/)) && argv_pos=argv_pos+2 ) && CHECK; }
	else return EXIT_FAILURE;
	}
}

int check_name(char file_name, const char * const * parms, int argv_pos /*TODO*/) {

	if(fnmatch(parms[argv_pos+1],file_name,FNM_NOESCAPE) == 0) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}

}

int check_path(char file_name, const char * const * parms, int argv_pos /*TODO*/) {

	if(fnmatch(parms[argv_pos+1],file_name,FNM_NOESCAPE) == 0) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}

}

/*
 * =================================================================== eof ==
 */

