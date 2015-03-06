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
#include <unistd.h>

/*
 * --------------------------------------------------------------- defines --
 */

/* Contant Definitions */
#define NAME_MAX	255
#define PATH_MAX	4096

#define MATCH		1
#define MISMATCH	0

#define NOPARAMETER (parms[parm_pos] == NULL)
#define NEXT_PARAMETER (check_next_parameter(parms[parm_pos+1]))
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
void check_stdout(void);

/*TODO: Tom */
int check_name(const char * file_name, const char * const * parms, int parm_pos);
int check_path(const char * file_name, const char * const * parms, int parm_pos);
int check_next_parameter(const char * string);


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

	/*check_stdout();*/
	/* Check if parameters are correct */
	if (check_arg(argc, argv) == EXIT_FAILURE) exit(EXIT_FAILURE);

	/* check (all) first dirs */
	do_file(filename, paramlist);

	/* Go through all files 
	do_dir(filename, paramlist); */

	exit(EXIT_SUCCESS);
}


/* Function Definitions */
int check_next_parameter(const char * string) {
	if (string[0] == '-') return MISMATCH;
	else return MATCH;
}

int check_arg(const int argc, const char * argv[]) {
	const char * const * parms = argv;
	int parm_pos = 2;

	if (argc < 2) {
		usage();
		return EXIT_FAILURE;
}

	while (parm_pos < argc) {
		if (NAME && NEXT_PARAMETER) parm_pos += 2;
		else if (PATH && NEXT_PARAMETER) parm_pos += 2;
		else if (USER && NEXT_PARAMETER) parm_pos += 2;
		else if (TYPE && NEXT_PARAMETER) {
			if (check_arg_type(argv[parm_pos + 1])) parm_pos += 2;
			}
		else if NOUSER parm_pos++;
		else if PRINT parm_pos++;
		else if LS parm_pos++;
		else {
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
			usage();
			return EXIT_FAILURE;
	}
}

void do_dir(const char * dir_name, const char * const * parms) {
	DIR *dir;
	struct dirent *d;
	char filename[PATH_MAX];

	if ((dir = opendir(dir_name)) == NULL) {
		if (errno != EACCES) {
			error(1, 1, "%d", errno);
			exit(EXIT_FAILURE);
		}
	} else {
		while ((d = readdir(dir)) != NULL) {
			if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;
			else {

				strcpy(filename, dir_name);
				/* if there is already a /, dont catenate again */
				if (dir_name[strlen(dir_name) - 1] != '/') {
					strcat(filename, "/");
				}
				strcat(filename, d->d_name);

				do_file(filename,parms);
			}

		}
	}

	closedir(dir);
}

void do_file(const char * file_name, const char * const * parms) {
	int parm_pos = 0;
	/* create filedescriptor */
	struct stat fd_in;

	/* lstat file and write content to struct
	 * throw error if failed with Filename */
	if (lstat(file_name, &fd_in) == -1) {
		error(1, 1, "%d", errno);
		exit(EXIT_FAILURE);
	}

	
	check(file_name, fd_in, parms, parm_pos);
	if NOPARAMETER {
		print(file_name);}
	if (S_ISDIR(fd_in.st_mode)) { do_dir(file_name,parms); }
}

int check(const char * dir_name, struct stat file, const char * const * parms, int parm_pos) {

	if NOPARAMETER {
		return MISMATCH;
	}
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
			if (!CHECK)  print(dir_name);
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

	/* TODO: Error checking */
	userdet = getpwuid(fd_in.st_uid);
	parmsint = strtol(parms[parm_pos + 1], &endptr, 10);

	if((strcmp(userdet->pw_name, parms[parm_pos + 1]) == 0) || (userdet->pw_uid == (uid_t)parmsint)) {
		return MATCH;
	} else {
		return MISMATCH;
	}


}

int check_nouser(struct stat fd_in)
{
	struct passwd *userdet = NULL;
	userdet = getpwuid(fd_in.st_uid);
	if(userdet == NULL) return MATCH;
	else return MISMATCH;

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

	struct passwd * passwd = getpwuid(file.st_uid);
	struct group * group = getgrgid(file.st_gid);

	/* Format mtime */
	char mtime[NAME_MAX];
	struct tm *time;

	time = localtime(&file.st_mtime);
	strftime(mtime, 1000, "%b %d %H:%M", time);

	if (fprintf(stdout, " %ld %4.0f ",
		(long) file.st_ino,		/* Inode */
		(double)file.st_blocks	) < 0) {
			fprintf(stderr, "Writung to stdout not possible\n");
			exit(EXIT_FAILURE);
	}
	/* Permissions */
	fprintf(stdout, (S_ISDIR(file.st_mode)) ? "d" : "-");
	fprintf(stdout, (file.st_mode & S_IRUSR) ? "r" : "-");
	fprintf(stdout, (file.st_mode & S_IWUSR) ? "w" : "-");
	fprintf(stdout, (file.st_mode & S_IXUSR) ? "x" : "-");
	fprintf(stdout, (file.st_mode & S_IRGRP) ? "r" : "-");
	fprintf(stdout, (file.st_mode & S_IWGRP) ? "w" : "-");
	fprintf(stdout, (file.st_mode & S_IXGRP) ? "x" : "-");
	fprintf(stdout, (file.st_mode & S_IROTH) ? "r" : "-");
	fprintf(stdout, (file.st_mode & S_IWOTH) ? "w" : "-");
	fprintf(stdout, (file.st_mode & S_IXOTH) ? "x" : "-");
	fprintf(stdout," %3ld %s %8s %12.0f %s %s\n",
		(long) file.st_nlink,		/* number of links */
		passwd->pw_name,		/* user name */
		group->gr_name,			/* group name */
		(double) file.st_size,		/* file size */
		mtime,		/* last modification date */
		file_name			/* file name */
	);

	return MATCH;
}

int print(const char * file_name) {
	fprintf(stdout, "%s\n", file_name);
	return MATCH;
}

/* deactivated, stops from compiling
void check_stdout(void) {
	if(!puts(isatty(fileno(stdout))) {
		error(1, 0, "%d", errno);
	}

	return 0;
} */

void usage (void) {
	fprintf(stderr, "\nUsage: myfind <directory> [ <options> ] ...\n\n");
	fprintf(stderr, "myfind is a simple command line tool for getting files and folders\n");
	fprintf(stderr, "under the specified path matching the given options.\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-name\t\t<pattern>\tfind files/directories with matching name\n");
	fprintf(stderr, "\t-user\t\t<name>/<uid>\tfind files/directories of a user\n");
	fprintf(stderr, "\t-nouser\t\t\t\tfind files/directories without a user\n");
	fprintf(stderr, "\t-path\t\t<pattern>\tfind files/directories with matchin path (incl. name)\n");
	fprintf(stderr, "\t-type\t\t[bcdpfls]\tfind files/directories with matching type\n");
	fprintf(stderr, "\t-print\t\t\t\tprint name of file/directory to console\n");
	fprintf(stderr, "\t-ls\t\t\t\tprint number of inode, number of blocks, permissions, etc...\n\n");
}

/*
 * =================================================================== eof ==
 */
