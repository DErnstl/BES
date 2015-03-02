#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <errno.h>
#include <string.h>
#include <fnmatch.h>
#include <string.h>
#include <pwd.h>
#include <dirent.h>

#include <err.h>
#include <error.h>
#include <errno.h>

/* Define constants */
#define MAX_NAME    255

#define MATCH       1
#define MISMATCH    0

#define PRINT_A     1
#define LS_A        2
#define NAME_A      3
#define TYPE_A      4
#define PATH_A      5
#define USER_A      6
#define NOUSER_A    7

/* Function Prototypes */
/* provided */
void do_dir(const char * dir_name, const char * const * parms);
void do_file(const char * file_name, const char * const * parms);
/* custom */
void arg_check(int argc, const char * argv[]);
int arg_type(const char * argv);
int get_parms_lenght(const char * const * parms);
int check(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght);
int action_check(const char * action, int parms_lenght);

int print(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght);
int check_name(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght);
int check_type(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght);

void print_usage(void);

/* Main function */
int main(int argc, const char * argv[]) {
    const char * const * parms = (const char * const *) &argv[2];

    arg_check(argc, argv);
    do_dir(argv[1], parms);

    exit(EXIT_SUCCESS);
}

/* Function Definitions */
void do_dir(const char * dir_name, const char * const * parms) {
    DIR * dir;
    struct dirent * dir_entry;
    char fname[MAX_NAME];

    if ( !(dir = opendir(dir_name)) ) {
        fprintf(stderr, "[ERROR] : opendir : %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    if ( !(dir_entry = readdir(dir)) ) {
        closedir(dir);
        fprintf(stderr, "[ERROR] : readdir : %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    do {
        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
            strcpy(fname, dir_name);
            strcat(fname, "/");
            strcat(fname, dir_entry->d_name);

            do_file(fname, parms);
        }
    } while ((dir_entry = readdir(dir)));
    
    closedir(dir);
}

void do_file(const char * file_name, const char * const * parms) {
    struct stat file_info;
    int print_counter = 0;
    int * print_p = &print_counter;
    int parms_pos = 0;
    int * parms_p = &parms_pos;
    int parms_lenght = get_parms_lenght(parms);
    int check_result = 0;

    lstat(file_name, &file_info);

    if (lstat(file_name, &file_info) == -1) {
        error(1, 0, " do_file : lstat");
        exit(EXIT_FAILURE);
    }

    /* WORK START */
    /*fprintf(stdout, "[LISTING] : do_dir : %s\n", file_name);*/

    
    check_result = check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
    if (check_result && !print_counter) {
        fprintf(stdout, "[INFO] do_file : printing because of no print/ls argument\n");
        print(file_name, file_info, parms, parms_p, print_p, parms_lenght);
    }
    /* WORK END */
    if (S_ISDIR(file_info.st_mode)) {
        do_dir(file_name, parms);
    }
}

int check(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght) {
    switch (action_check(parms[(*parms_p)], parms_lenght)) {
        case PRINT_A:
            print(file_name, file_info, parms, parms_p, print_p, parms_lenght);
            return MATCH;
            break;
        case NAME_A:
            return check_name(file_name, file_info, parms, parms_p, print_p, parms_lenght);
            break;
        case TYPE_A:
            return check_type(file_name, file_info, parms, parms_p, print_p, parms_lenght);
            /*
            if (check_type(file_name, file_info, parms, parms_p, print_p, parms_lenght) && *print_p == 0) {
                print(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                return MATCH;
            } else {
                    return MISMATCH;
            }
            */
            break;
        default:
            /*
            if (print_counter == 0) {
                fprintf(stdout, "[WARNING] : check : printing from check->default\n");
                print(file_name, parms_p, print_p, parms_lenght);
                return MATCH;
            }
            */
            return MISMATCH;
    }

}

int print(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght) {
    int index = *parms_p;
    fprintf(stdout, "%s\n", file_name);
    (*print_p)++;
    if (index + 1 < parms_lenght) {
        (*parms_p)++;
        check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
    }
    return MATCH;
}

int check_name(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght) {
    int index = *parms_p;

    /*fprintf(stdout, "[DEBUG]%d\n", index);*/

    if (!strcmp(file_name, parms[index + 1])) {
        if (index + 2 < parms_lenght) {
            *parms_p += 2;
            return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
        } else {
            return MATCH;
        }
    } else {
        return MISMATCH;
    }
}

int check_type(const char * file_name, struct stat file_info, const char * const * parms, int * parms_p, int * print_p, int parms_lenght) {
    int index = *parms_p;
    char type = (char) *parms[++index];

    switch (type) {
        case 'b':
            if (S_ISBLK(file_info.st_mode)) {
                if (index + 1 < parms_lenght) {
                    *parms_p += 2;
                    return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                } else {
                    return MATCH;
                }
            } else {
                return MISMATCH;
            }
            break;
        case 'c':
            if (S_ISCHR(file_info.st_mode) && (*parms_p + 2 <= parms_lenght)) {
                if (index + 1 < parms_lenght) {
                    *parms_p += 2;
                    return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                } else {
                    return MATCH;
                }
            } else {
                return MISMATCH;
            }
            break;
        case 'd':
            if (S_ISDIR(file_info.st_mode)) {
                if (index + 1 < parms_lenght) {
                    *parms_p += 2;
                    return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                } else {
                    return MATCH;
                }
            } else {
                return MISMATCH;
            }
            break;
        case 'p':
            if (S_ISFIFO(file_info.st_mode) && (*parms_p + 2 <= parms_lenght)) {
                if (index + 1 < parms_lenght) {
                    *parms_p += 2;
                    return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                } else {
                    return MATCH;
                }
            } else {
                return MISMATCH;
            }
            break;
        case 'f':
            if (S_ISREG(file_info.st_mode) && (*parms_p + 2 <= parms_lenght)) {
                if (index + 1 < parms_lenght) {
                    *parms_p += 2;
                    return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                } else {
                    return MATCH;
                }
            } else {
                return MISMATCH;
            }
            break;
        case 'l':
            if (S_ISLNK(file_info.st_mode) && (*parms_p + 2 <= parms_lenght)) {
                if (index + 1 < parms_lenght) {
                    *parms_p += 2;
                    return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                } else {
                    return MATCH;
                }
            } else {
                return MISMATCH;
            }
            break;
        case 's':
            if (S_ISSOCK(file_info.st_mode) && (*parms_p + 2 <= parms_lenght)) {
                if (index + 1 < parms_lenght) {
                    *parms_p += 2;
                    return check(file_name, file_info, parms, parms_p, print_p, parms_lenght);
                } else {
                    return MATCH;
                }
            } else {
                return MISMATCH;
            }
            break;
        default:
            error(1, 0, "check : default error");
            return MISMATCH;
    }
}

/* "Done" */
void arg_check(int argc, const char * argv[]) {
    int arg_i = 2;

    if (argc < 2) {
        error(1, 0, "parms_check : Insufficient arguments");
        print_usage();
        exit (EXIT_FAILURE);
    }

    while (arg_i < argc) {
        if ((strcmp(argv[arg_i], "-name") == 0) && (arg_i + 1 < argc)) {
            arg_i += 2;
            fprintf(stderr, "[INFO] : parms_check : NAME OK\n");
        } else if ((strcmp(argv[arg_i], "-type") == 0) && (arg_i + 1 < argc)) {
            if (arg_type(argv[arg_i + 1])) {
                arg_i += 2;
                fprintf(stderr, "[INFO] : parms_check : TYPE OK\n");
            } else {
                error(1, 0, "parms_check : Invalid arguments");
                print_usage();
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[arg_i], "-print") == 0) {
            arg_i++;
            fprintf(stderr, "[INFO] : parms_check : PRINT OK\n");
        } else {
            error(1, 0, "parms_check : Invalid arguments");
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

    /* OLD */
    /*
    if ((strlen(argv) == 1) &&
        (((char) *argv == 'b') ||
        ((char) *argv == 'c') ||
        ((char) *argv == 'd') ||
        ((char) *argv == 'p') ||
        ((char) *argv == 'f') ||
        ((char) *argv == 'l') ||
        ((char) *argv == 's'))) {
        return MATCH;
    } else {
        return MISMATCH;
    }
    */
}

int get_parms_lenght(const char * const * parms) {
    int parms_lenght = 0;
    int i = 0;
    while (parms[i] != NULL) {
        parms_lenght++;
        i++;
    }

    return parms_lenght;
}

int action_check(const char * action, int parms_lenght) {
    if (parms_lenght == 0 || (strcmp(action, "-print") == 0)) {
        return PRINT_A;
    } else if (strcmp(action, "-ls") == 0) {
        return LS_A;
    } else if (strcmp(action, "-name") == 0) {
        return NAME_A;
    } else if (strcmp(action, "-type") == 0) {
        return TYPE_A;
    } else if (strcmp(action, "-path") == 0) {
        return PATH_A;
    } else if (strcmp(action, "-user") == 0) {
        return USER_A;
    } else if (strcmp(action, "-nouser") == 0) {
        return NOUSER_A;
    } else {
        return MISMATCH;
    }
}

void print_usage(void) {
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
