/**
 * @file myfind.c
 * Betriebssysteme myfind
 * Beispiel 1
 *
 * @author Adam Kerenyi <ic14b080@technikum-wien.at>
 * @author Romeo Beck <ic14b037@technikum-wien.at>
 * @author Thomas Zeitinger <ic14b033@technikum-wien.at>
 * @date 2015/02/09
 *
 * @version 470
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
#include <pwd.h>
#include <sys/stat.h>
#include <dirent.h>


/*
 * --------------------------------------------------------------- defines --
 */

/* Contant Definitions */
#define MAXNAMELENGHT 255

#define MATCH		1
#define MISMATCH	0

#define NOPARAMETER (strcmp(parms[argv_pos], "\0") == 0)
#define NAME (strcmp(parms[argv_pos], "-name") == 0)
#define PATH (strcmp(parms[argv_pos], "-path") == 0)
#define USER (strcmp(parms[argv_pos], "-user") == 0)
#define NOUSER (strcmp(parms[argv_pos], "-nouser") == 0)
#define TYPE (strcmp(parms[argv_pos], "-type") == 0)
#define LS (strcmp(parms[argv_pos], "-ls") == 0)
#define PRINT (strcmp(parms[argv_pos], "-print") == 0)

#define CHECK (check(file_name,fd_in,parms,argv_pos))

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */

/*
 * ------------------------------------------------------------- functions --
 */


/* Function Prototypes */
void do_file(const char * file_name, const char * const * parms);	/* return type void as none was given */
void do_dir(const char * dir_name, const char * const * parms);		/* return type void as none was given */
int check(const char * file_name, struct stat file, const char * const * parms, int argv_pos);

/*TODO: Romeo */
int check_user(struct stat fd_in, const char * const * parms, int argv_pos);
int check_nouser(struct stat fd_in, const char * const * parms, int argv_pos);
/*int check_parameters();*/	/* replaced by arg_check() */
void arg_check(int argc, const char * argv[]);
int arg_type(const char * argv);


/*TODO: Adam */
int check_type(/*TODO*/);
int ls(const char * file_name, struct stat file);
int print(const char * file_name);
int usage (void); /*Adam*/

/*TODO: Tom */
int check_name(const char * file_name, const char * const * parms, int argv_pos);
int check_path(const char * file_name, const char * const * parms, int argv_pos);


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
	argc = argc;

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

void do_dir(const char * dir_name, const char * const * parms) {
	/* Create DIR Struct */
	DIR *dir;
	struct dirent *d;
	char filename[MAXNAMELENGHT];

	/* opendir an throw error with exename if error */
	if ((dir = opendir(dir_name)) == NULL ) {
		/*TODO, filename ist nicht mehr perms[0] */
			perror(parms[0]);
			exit(EXIT_FAILURE);
	}

	/* readdir until NULL */
	while ((d = readdir(dir)) != NULL ) {
		/* if dir_name == ".", then do_file, else cut "." and ".." out */
		if ((!strcmp(dir_name, ".")) && (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)) continue;
		else {  
			strcpy(filename, dir_name);
			strcat(filename, "/");
			strcat(filename, d->d_name);
			do_file(filename,parms);
		}

	}

	closedir(dir);
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

	check(file_name, fd_in, parms, argv_pos);
	if (S_ISDIR(fd_in.st_mode)) { do_dir(file_name,parms); }
}

int check(const char * dir_name, struct stat file, const char * const * parms, int argv_pos) {
	if NOPARAMETER return EXIT_FAILURE;
	else if NAME {( (check_name(dir_name,parms,argv_pos)) && (argv_pos=argv_pos+2) ) && ( CHECK || print(/*TODO*/) ); }
	else if PATH {( (check_path(dir_name,parms,argv_pos)) && (argv_pos=argv_pos+2) ) && ( CHECK || print(/*TODO*/) ); }
	else if USER {( (check_user(file,parms,argv_pos)) && (argv_pos=argv_pos+2) ) && ( CHECK || print(/*TODO*/) ); }
	else if NOUSER {( (check_nouser(file,parms,argv_pos)) && (argv_pos++) ) && ( CHECK || print(/*TODO*/) ); }
	else if TYPE {( (check_type(/*TODO*/)) && (argv_pos=argv_pos+2) ) && ( CHECK || print(/*TODO*/) ); }
	else if LS {( (print_ls(dir_name, file)) && (argv_pos=argv_pos+2) ) && CHECK; }
	else if PRINT {( (print(dir_name)) && (argv_pos=argv_pos+2) ) && CHECK; }
	else return EXIT_FAILURE;
}

int check_name(const char * file_name, const char * const * parms, int argv_pos) {
	if(fnmatch(parms[argv_pos+1],file_name,FNM_NOESCAPE) == 0) {
		return MATCH;
	} else {
		return MISMATCH;
	}

}

int check_path(const char * file_name, const char * const * parms, int argv_pos) {
	if(fnmatch(parms[argv_pos+1],file_name,FNM_PATHNAME) == 0) {
		return MATCH;
	} else {
		return MISMATCH;
	}

}

int check_user(struct stat fd_in, const char * const * parms, int argv_pos)
{
	struct passwd *userdet = NULL;
	userdet = getpwuid(fd_in.st_uid);
	char *endptr = NULL;
	int parmsint = 0;
	parmsint = strtol(parms[argv_pos], &endptr, 10);		
	if(((userdet->pw_name) == (parms[argv_pos + 1])) || (userdet->pw_uid == parmsint)) return EXIT_SUCCESS;
	else return EXIT_FAILURE;
	
}

int check_nouser(struct stat fd_in, const char * const * parms, int arg_pos)
{
	struct passwd *userdet = NULL;
	userdet = getpwuid(fd_in.st_uid);
	if(userdet == NULL) return EXIT_SUCCESS;
	else return EXIT_FAILURE;

}

int ls(const char * file_name, struct stat file) {
	/*20696685        8 -rw-r--r--    1 akerenyi         staff                1453 Mar  2 18:19 ./Makefile*/
	/*indoe			???	permissions	 link	user		group				  size last mod. date	file name*/

	struct passwd * user = NULL;
	user = getpwuid(file_info.st_uid);

	/*getgrgid*/

	fprintf(stdout, "%8ld\t???\t%.0f %s %s\n", (long) file_info.st_ino, (long) file_info.st_nlink), user->pw_name, (double) file_info.st_size, ctime(&file_info.st_mtime), file_name);
	return MATCH;

}

int print(const char * file_name) {
	fprintf(stdout, "%s\n", file_name);
	return MATCH;
}

void arg_check(int argc, const char * argv[]) {
    int arg_i = 2;

    if (argc < 2) {
        error(1, 0, "Insufficient arguments");
        print_usage();
        exit (EXIT_FAILURE);
    }

    while (arg_i < argc) {
        if ((strcmp(argv[arg_i], "-name") == 0) && (arg_i + 1 < argc)) {
            arg_i += 2;
        } else if (!(strcmp(argv[arg_i], "-path")) && (arg_i + 1 < argc)) {
            arg_i += 2;
        } else if (!(strcmp(argv[arg_i], "-user")) && (arg_i + 1 < argc)) {
            arg_i += 2;
        } else if (!(strcmp(argv[arg_i], "-nouser"))) {
            arg_i++;
        } else if ((strcmp(argv[arg_i], "-type") == 0) && (arg_i + 1 < argc)) {
            if (arg_type(argv[arg_i + 1])) {
                arg_i += 2;
            } else {
                error(1, 0, "Invalid arguments");
                print_usage();
                exit(EXIT_FAILURE);
            }
        } else if (!(strcmp(argv[arg_i], "-print"))) {
            arg_i++;
        } else if (!(strcmp(argv[arg_i], "-ls"))) {
            arg_i++;
        } else {
            error(1, 0, "Invalid arguments");
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
}

int arg_type(const char * argv) {
    switch ((char) * argv) {
        case 'b':
        case 'c':
        case 'd':
        case 'p':
        case 'f':
        case 'l':
        case 's':
            return MATCH;
            break;
        default:
            return MISMATCH;
    }
}

/*
 * =================================================================== eof ==
 */

