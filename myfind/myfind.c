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
#include <error.h>
#include <string.h>
#include <fnmatch.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>

/*
 * --------------------------------------------------------------- defines --
 */

/* Contant Definitions */
#define NAME_MAX	255
#define PATH_MAX	4096

#define MATCH		1
#define MISMATCH	0

#define NOPARAMETER (strcmp(parms[parm_pos], "\0") == 0)
#define NAME (strcmp(parms[parm_pos], "-name") == 0)
#define PATH (strcmp(parms[parm_pos], "-path") == 0)
#define USER (strcmp(parms[parm_pos], "-user") == 0)
#define NOUSER (strcmp(parms[parm_pos], "-nouser") == 0)
#define TYPE (strcmp(parms[parm_pos], "-type") == 0)
#define LS (strcmp(parms[parm_pos], "-ls") == 0)
#define PRINT (strcmp(parms[parm_pos], "-print") == 0)

#define CHECK (check(dir_name,file,parms,parm_pos))

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
int check(const char * file_name, struct stat file, const char * const * parms, int parm_pos);

/*TODO: Romeo */
int check_user(struct stat fd_in, const char * const * parms, int parm_pos);
int check_arg(const int argc, const char * argv[]);
int check_arg_type(const char * argv);
int check_nouser(struct stat fd_in);


/*TODO: Adam */
int check_type(struct stat file, const char * const * parms, int parm_pos);
int print_ls(const char * file_name, struct stat file);
int print(const char * file_name);
void usage (void); /*Adam*/

/*TODO: Tom */
int check_name(const char * file_name, const char * const * parms, int parm_pos);
int check_path(const char * file_name, const char * const * parms, int parm_pos);


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
	const char * const *paramlist = (const char * const *)&argv[2];
	const char *filename = (const char *)argv[1];

	/* Check if parameters are correct */
	if (check_arg(argc, argv) == EXIT_FAILURE) exit(EXIT_FAILURE);

	/* Go through all files */
printf("main() filename: %s\n", filename);
	do_dir(filename, paramlist);

	exit(EXIT_SUCCESS);
}


/* Function Definitions */
int check_arg(const int argc, const char * argv[]) {
	const char * const * parms = argv;
	int parm_pos = 2;

	if (argc < 2) {
	error(1, 1, "%d", errno);
	usage();
	return EXIT_FAILURE;
}

	while (parm_pos < argc) {
		if((!NAME || !PATH || !USER) && (parm_pos + 1 < argc)) {
			parm_pos += 2;
		} else if(!NOUSER || !PRINT || !LS) {
			parm_pos++;
		} else if (!TYPE && (parm_pos + 1 < argc)) {
			if (check_arg_type(argv[parm_pos + 1])) {
				parm_pos += 2;
			} else {
				error(1, 1, "%d", errno);
				usage();
				return EXIT_FAILURE;
			}
		} else {
			error(1, 1, "%d", errno);
			usage();
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int check_arg_type(const char * argv) {
   	switch ((char) * argv) {
		case 'b':
		case 'B':
		case 'c':
		case 'C':
		case 'd':
		case 'D':
		case 'p':
		case 'P':
		case 'f':
		case 'F':
		case 'l':
		case 'L':
		case 's':
		case 'S':
			    return MATCH;
			break;
		default:
			return MISMATCH;
	}
}

void do_dir(const char * dir_name, const char * const * parms) {
	/* Create DIR Struct */
	DIR *dir;
	struct dirent *d;
	char filename[PATH_MAX];

printf("do_dir from main(): dir_name: %s\n", dir_name);
	/* opendir an throw error with exename if error */
	if ((dir = opendir(dir_name)) == NULL) {
		/*TODO, filename ist nicht mehr perms[0] */
			error(1, 1, "%d", errno);
			exit(EXIT_FAILURE);
	}

printf("do_dir after opendir: dir_name: %s\n", dir_name);
	while ((d = readdir(dir)) != NULL) {
		/* if dir_name == ".", then do_file, else cut "." and ".." out */
printf("d->d_name: %s\n", d->d_name);
		if ((!strcmp(dir_name, ".")) && (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)) continue;
		else {
printf("filename 1: %s\n", filename);
			strcpy(filename, dir_name);
printf("filename 2: %s\n", filename);
			strcat(filename, "/");
printf("filename 3: %s\n", filename);
			strcat(filename, d->d_name);
printf("filename 4: %s\n", filename);
			do_file(filename,parms);
		}

	}

	closedir(dir);
}

void do_file(const char * file_name, const char * const * parms) {
	int parm_pos = 2;
	/* create filedescriptor */
	struct stat fd_in;

	/* lstat file and write content to struct
	 * throw error if failed with Filename */
	if (lstat(file_name, &fd_in) == -1) {
		error(1, 1, "%d", errno);
		exit(EXIT_FAILURE);
	}

	check(file_name, fd_in, parms, parm_pos);
	if (S_ISDIR(fd_in.st_mode)) { do_dir(file_name,parms); }
}

int check(const char * dir_name, struct stat file, const char * const * parms, int parm_pos) {

printf("in check()\n");

	if NOPARAMETER return MISMATCH;
	else if NAME {
		if (check_name(dir_name,parms,parm_pos)) {
			parm_pos=parm_pos+2;
			if (!CHECK) print(dir_name);
		}
	}
	else if PATH {
		if (check_path(dir_name,parms,parm_pos)) {
			parm_pos=parm_pos+2;
			if (!CHECK) print(dir_name);
		}
	}
	else if USER {
		if (check_user(file,parms,parm_pos)) {
			parm_pos=parm_pos+2;
			if (!CHECK) print(dir_name);
		}
	}
	else if NOUSER {
		if (check_nouser(file)) {
		       parm_pos++;
		       if (!CHECK) print(dir_name);
		}
	}
	else if TYPE {
		if (check_type(file, parms, parm_pos)) {
			parm_pos=parm_pos+2;
			if (!CHECK) print(dir_name);
		}
	}
	else if LS {
		if (print_ls(dir_name, file)) {
			parm_pos=parm_pos+2;
			CHECK;
		}
	}
	else if PRINT {
		if (print(dir_name)) {
			parm_pos=parm_pos+2;
			CHECK;
		}
	}

	return MISMATCH;
}

int check_name(const char * file_name, const char * const * parms, int parm_pos) {
	if(fnmatch(parms[parm_pos+1],file_name,FNM_NOESCAPE) == 0) {
		return MATCH;
	} else {
		return MISMATCH;
	}

}

int check_path(const char * file_name, const char * const * parms, int parm_pos) {
	if(fnmatch(parms[parm_pos+1],file_name,FNM_PATHNAME) == 0) {
		return MATCH;
	} else {
		return MISMATCH;
	}

}

int check_user(struct stat fd_in, const char * const * parms, int parm_pos)
{
	struct passwd *userdet = NULL;
	char *endptr = NULL;
	int parmsint = 0;
	userdet = getpwuid(fd_in.st_uid);
	parmsint = strtol(parms[parm_pos + 1], &endptr, 10);
	if(((userdet->pw_name) == (parms[parm_pos + 1])) || (userdet->pw_uid == (uid_t)parmsint)) return EXIT_SUCCESS;
	else return EXIT_FAILURE;

}

int check_nouser(struct stat fd_in)
{
	struct passwd *userdet = NULL;
	userdet = getpwuid(fd_in.st_uid);
	if(userdet == NULL) return EXIT_SUCCESS;
	else return EXIT_FAILURE;

}

int check_type(struct stat file, const char * const * parms, int parm_pos) {
	char type = (char) *parms[parm_pos];

	switch (type) {
		case 'b':
		case 'B':
			if (S_ISBLK(file.st_mode)) {
				return MATCH;
			} else {
				return MISMATCH;
			}
			break;
		case 'c':
		case 'C':
			if (S_ISCHR(file.st_mode)) {
				return MATCH;
			} else {
				return MISMATCH;
			}
			break;
		case 'd':
		case 'D':
			if (S_ISDIR(file.st_mode)) {
				return MATCH;
			} else {
				return MISMATCH;
			}
			break;
		case 'p':
		case 'P':
			if (S_ISFIFO(file.st_mode)) {
				return MATCH;
			} else {
				return MISMATCH;
			}
			break;
		case 'f':
		case 'F':
			if (S_ISREG(file.st_mode)) {
				return MATCH;
			} else {
				return MISMATCH;
			}
			break;
		case 'l':
		case 'L':
			if (S_ISLNK(file.st_mode)) {
				return MATCH;
			} else {
				return MISMATCH;
			}
			break;
		case 's':
		case 'S':
			if (S_ISSOCK(file.st_mode)) {
				return MATCH;
			} else {
				return MISMATCH;
			}
			break;
		default:
			error(1, 1, "%d", errno);
			return MISMATCH;
	}
}

int print_ls(const char * file_name, struct stat file) {
	/*20696685	8 -rw-r--r--	1 akerenyi	staff	1453 Mar  2 18:19 ./Makefile*/
	/*indoe			???	permissions	 link	user		group				  size last mod. date	file name*/

	struct passwd * passwd = getpwuid(file.st_uid);
	struct group * group = getgrgid(file.st_gid);
	/*getgrgid*/

	fprintf(stdout, "%8ld\t???\t%ld\t%s\t%s\t%.0f %s %s\n",
		(long) file.st_ino,		/* inode */
		/* MISSING! size in blocks ??? */
		(long) file.st_nlink,		/* number of links */
		passwd->pw_name,					/* user name */
		group->gr_name,					/* group name */
		(double) file.st_size,		/* file size */
		ctime(&file.st_mtime),		/* last modification date */
		file_name						/* file name */
	);

	return MATCH;
}

int print(const char * file_name) {
	fprintf(stdout, "%s\n", file_name);
	return MATCH;
}


/*
 * =================================================================== eof ==
 */
