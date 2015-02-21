/* =================== */
/* === PSEUDO CODE === */
/* =================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>		/* getpwnam("username") || getpwuid(uid_t uid) */
#include <sys/stat.h>	/* lstat() -> -print(), -ls() */
#include <dirent.h>		/* DIR *opendir(const char* dirname) //full of relative path */

/* Contant Definitions */
#define PRINT	0
#define LS		1 
#define USER	2
#define NOUSER	3
#define PATH	4
#define NAME	5
#define TYPE	6
#define MAXNAMELENGHT 256

/* Function Prototypes */
void do_file(const char * file_name, const char * const * parms);	/* return type void as none was given */
void do_dir(const char * dir_name, const char * const * parms);		/* return type void as none was given */
int check_parameters();	/* MISSING: add parameters */
int prepare_path();	/* MISSING: add parameters */
int check(char file_name, const char * const * parms, int position);

/* Main Function */
int main(int argc, char *argv[]) {

	/* We want argv as constant */
	const char * const *paramlist  = (const char * const *)&argv[0];

	/* Declare variables */
	DIR *stream = NULL;
	struct dirent * files = NULL;
	int arg_pos = 0;
	char * path;

	/* Check if parameters are correct */
	check_parameters();

	/* Check and prepare path if necessary */
	prepare_path(char * path);
	stream = opendir(path);
	if (stream == NULL) {
		fprintf(stderr, "[ERROR] myfind: %3d: %s\n", errno, strerror(errno));
		/* or without erro number: perror ("[ERROR] myfind"); */
		exit(EXIT_FAILURE);
	}

	/* Go through all files */

	/* Main function below this point not checkd */
	while ((files = readdir(stream)) != NULL) {
		fprintf("%s\n", files->d_name);
	}

	closedir(stream);


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
}
int check(char file_name, const char * const * parms, int position) {
	switch(parms[position] {
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
