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
void prepare_path();	/* MISSING: add parameters */

/* Main Function */
int main(int argc, conts char *argv[]) {

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

void prepare_path(char * path) {
	/* Check if path argv[1] is set and if it is a directory */
}
