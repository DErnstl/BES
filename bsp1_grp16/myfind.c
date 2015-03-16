/**
 * @file myfind.c
 * Betriebssysteme myfind programm
 * Beispiel 1
 *
 * @author 	Stefan Schmidt <ic14b066@technikum-wien.at>
 * 		Christoph Birkner <ic14b005@technikum-wien.at>
 * 		Patrik Habe <ic14b044@technikum-wien.at>
 * @date 2015/03/09
 *
 * @version 1.9
 *
 *
 */

/*
 * -------------------------------------------------------------- includes --
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <error.h>
#include <errno.h>
#include <fnmatch.h>
#include <libgen.h>

/*
 * --------------------------------------------------------------- defines --
 */
/*!brief maximal length of path*/
#define NO_FILE_MATCH -1
#define NO_USER_MATCH 1
#define MEMORY_ERROR 99
#define DONTCHECKUIDFLAG -1
#define DOCHECKUIDFLAG -2
/*! \brief type flags*/
#define TYPE_FLAG_B 'b'
#define TYPE_FLAG_C 'c'
#define TYPE_FLAG_D 'd'
#define TYPE_FLAG_P 'p'
#define TYPE_FLAG_F 'f'
#define TYPE_FLAG_L 'l'
#define TYPE_FLAG_S 's'
/*
 * -------------------------------------------------------------- typedefs --
 */


enum {
	NAME_FLAG =   (1 << 0),
	USER_FLAG =   (1 << 1),
	PATH_FLAG =   (1 << 2),
	TYPE_FLAG =   (1 << 3),
	PRINT_FLAG =  (1 << 4),
	LS_FLAG =     (1 << 5),
	NOUSER_FLAG = (1 << 6)
};
/* 
 * \brief Structure params which hold each argv parameter in a structure
 */
struct params {
	int paramtype;
	struct params *next;
	char param_value[1];
};

/*
 * --------------------------------------------------------------- globals --
 */
 /* ### FB: globale variable ist nicht so schön, wenn dann static 
  * 	und MAX_FILENAME ist sicher größer als 100 
  * 	siehe /usr/src/kernels/$(uname -r)/include/linux/limits.h */
char programmname [100]; /* weils mich grad nicht gfreut das sauber zu machen TODO */
/*
 * ------------------------------------------------------------- functions --
 */

char *strerror( int errnum ); /*! generates typical error message */
int do_file(const char *filename, const struct params *parameters); 			/*! process a file */
int do_directory(const char *pathname, const struct params *parameters); 	/*! process a directory. calls do_file */
int do_output(const char *fullfilename, const struct params *parameters, const struct stat *file_stat); /*! format all fields and do print or ls */
void do_usageprint(int postusage);	/*! Print out usage of myfind */
void get_file_attribs(const struct stat *file_stat); /*! get rights of file */
void gettime(const time_t timep); /*! get formated time */
struct passwd* getuser(const char *username, const long fileuid); /*! checks /etc/passwd for uid or username */
struct group* getgroup(const char *groupname, const long grpuid); /*! check /etc/groups for gid or groupname */
unsigned long makeuid(const char *username); /*! convert username into long value */
/**
 *
 * \brief The uberrealistic find simulations
 *
 * main is the point to start, where else?
 *
 * \param argc the number of arguments
 * \param argv the arguments itselves (including the program name in argv[0])
 *
 * \return 0 or in case of error 1
 * \retval 0 no error
 * \retval 1 User not found
 * \retval 99 Memory error
 *
 */

int main (int argc, const char *argv[])
{
	int run=0;
	int postusage=0;
	int return_value=0;
	int param_check=0;
	long uidcheck=0;	
	
	struct passwd *check_user=NULL;
	struct params *start=NULL;
	struct params *parameters=NULL;
	struct params *last=NULL;
	char defaultdir[2]=".";
	char *startdir=NULL;

	
	strncpy(programmname,argv[0],strlen(argv[0]));

	/* ### FB: wir finden das ist eine etwas lange main() :)
	 * 	und es werden zwei Funktionen ausgeführt. Eine
	 * 	der beiden, wohl am ehesten den Parametercheck,
	 * 	könnte man auslagern. */

	/*! brief initiate default values */	
	for(run=1;run<=argc;run++) {
		param_check=0;
		if (run==1) {
			if ((argv[run]!=NULL) && (argv[run][0]!='-')) { /* first parameter is the path where to start myfind */
				if ((startdir=malloc((sizeof(char)*strlen(argv[run])+1)))==NULL) {
					fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
					exit(MEMORY_ERROR);
				} else {
					strcpy(startdir,argv[run]);
					param_check=1;
				}
			} else { /* first parameter is not path so use . as path */
				startdir=defaultdir;
			}	
		}
		if (argv[run] == NULL) {  /* no further parameters and searchpath already checked so exit loop for parameter handling */
			break;
		}

		 /* ### FB: Im folgenden sind die Schleifen immer sehr ähnlich, ggf wäre eine
		  * 	Rekursion mit z.B. Übergabe des Flags in die Fkt kürzer und übersichtlicher */
		if (strncmp(argv[run],"-user",5)==0) {
			if ((argv[run+1]!=NULL) && (argv[run+1][0] != '-')) {
				 /* ### FB: ADAM: kannst Du Dir den malloc() ansehen? Ich
				  * 	verstehe es nicht komplett */
				if ((parameters=malloc(sizeof(struct params)+(sizeof(char)*strlen(argv[run+1])+1)))==NULL) {
					fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
					exit(MEMORY_ERROR);
				} else {
					if (((check_user= getuser(argv[run+1],DONTCHECKUIDFLAG))!=NULL) \
						 || ((uidcheck=makeuid(argv[run+1]))!=0)) {
						if (start==NULL) {
							start=parameters;
						} else {
							last->next=parameters;
						}
						parameters->next=NULL;
						last=parameters;
						parameters->paramtype=0 | USER_FLAG;
						strcpy(parameters->param_value,argv[run+1]);
						run++;
					} else {
						fprintf(stderr,"[%s]\t:Username: '%s' is not existing nor an UID.\n",programmname,argv[run+1]);
						postusage=1;
					}
					param_check=1;
				}	
			} else { /* bad parameter for user or no user parameter found. error handling */
				postusage=1;
				param_check=1;
			}
		}
		if ((strncmp(argv[run],"-name",5)==0)) {
			if ((argv[run+1]!=NULL)) {
				if ((parameters=malloc(sizeof(struct params)+(sizeof(char)*strlen(argv[run+1])+1)))==NULL) {
					fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
					exit(MEMORY_ERROR);
				} else {
					if (start==NULL) {
						start=parameters;
					} else {
						last->next=parameters;
					}
					parameters->next=NULL;
					last=parameters;
					parameters->paramtype=0 | NAME_FLAG;
					strcpy(parameters->param_value,argv[run+1]);
					run++;
				}	

			} else { /* bad parameter for name or no name parameter found. error handling */
				postusage=2;
			}
			param_check=1;

		}
		if ((strncmp(argv[run],"-path",5)==0)) {
			if ((argv[run+1]!=NULL) && (argv[run+1][0] != '-')) {
				if ((parameters=malloc(sizeof(struct params)+(sizeof(char)*strlen(argv[run+1])+1)))==NULL) {
					fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
					exit(MEMORY_ERROR);
				} else {
					if (start==NULL) {
						start=parameters;
					} else {
						last->next=parameters;
					}
					parameters->next=NULL;
					last=parameters;
					parameters->paramtype=0|PATH_FLAG;
					strcpy(parameters->param_value,argv[run+1]);
					run++;
				}	
			} else { /* bad parameter for path or no path parameter found. error handling */
				postusage=3;
			}
			param_check=1;

		}
		if ((strncmp(argv[run],"-type",5)==0)) {
			if ((argv[run+1]!=NULL) && (argv[run+1][0] != '-')) {
				if ((parameters=malloc(sizeof(struct params)+(sizeof(char)*strlen(argv[run+1])+1)))==NULL) {
					fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
					exit(MEMORY_ERROR);
				} else {
					if (start==NULL) {
						start=parameters;
					} else {
						last->next=parameters;
					}
					parameters->next=NULL;
					last=parameters;
					parameters->paramtype=0|TYPE_FLAG;
					if (	(argv[run+1][0]=='b') || \
						(argv[run+1][0]=='c') || \
						(argv[run+1][0]=='d') || \
						(argv[run+1][0]=='p') || \
						(argv[run+1][0]=='f') || \
						(argv[run+1][0]=='l') || \
						(argv[run+1][0]=='s')) {
				
						strncpy(parameters->param_value,argv[run+1],1);
						run++;
						param_check=1;
					} else {
						postusage=4;
					}
				}	
			} else { /* bad parameter for type or no type parameter found. error handling */
				postusage=4;
			}
		}
			
		if (strncmp(argv[run],"-print",6)==0) {
			if ((parameters=malloc(sizeof(struct params)+(sizeof(char)*7)))==NULL) {
				fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
				exit(MEMORY_ERROR);
			} else {
				if (start==NULL) {
					start=parameters;
				} else {
					last->next=parameters;
				}
				parameters->next=NULL;
				last=parameters;
				parameters->paramtype=0|PRINT_FLAG;
				strcpy(parameters->param_value,"-print");
			}	
			param_check=1;
		}
		if (strncmp(argv[run],"-ls",3)==0) {
			if ((parameters=malloc(sizeof(struct params)+(sizeof(char)*4)))==NULL) {
				fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
				exit(MEMORY_ERROR);
			} else {
				if (start==NULL) {
					start=parameters;
				} else {
					last->next=parameters;
				}
				parameters->next=NULL;
				last=parameters;
				parameters->paramtype=0|LS_FLAG;
				strcpy(parameters->param_value,"-ls");
			}	
			param_check=1;
		}
		if (strncmp(argv[run],"-nouser",7)==0) {
			if ((parameters=malloc(sizeof(struct params)+(sizeof(char)*8)))==NULL) {
				fprintf(stderr,"[%s]\t:Critical error on memory allocation. Exiting\n",argv[0]);
				exit(MEMORY_ERROR);
			} else {
				if (start==NULL) {
					start=parameters;
				} else {
					last->next=parameters;
				}
				parameters->next=NULL;
				last=parameters;
				parameters->paramtype=0|NOUSER_FLAG;
				strcpy(parameters->param_value,"-nouser");
			}	
			param_check=1;
		}
		if ((param_check==0) && (postusage==0)) {
			postusage=5;
		}	

	}
	if (postusage==0) {
		if((return_value=do_directory((const char *) startdir,start))==MEMORY_ERROR) {
			fprintf(stderr,"[%s]\t: Critical error on memory allocation. Exiting\n",programmname);
		}
	} else {
		do_usageprint(postusage);
		return_value=postusage;
	}

	for (parameters=start; parameters != NULL; ) {
		last=parameters;	
		parameters=parameters->next;
		free(last);
	}
	return return_value;
}	

/**
 *
 * \brief works on one file and do parameter checks
 *
 * Here every parameter is checked against the current file
 * If a directory is identified do_directory is called.
 *
 * \param filename is a string which includes the full path to the current file
 * \param struct params. The start point for the linked list of parameters
 *
 * \return 0 or in case of error 1
 * \retval 0 no error
 * \retval 1 User not found
 * \retval 99 Memory error
 *
 */
int do_file(const char *filename, const struct params *param) 
{
        struct stat attrib;                     /* create a file attribute structure */
	struct passwd *check_user;
	const struct params *running=NULL;
	int defaultprint=1;
	int cmp_value=0;
	char *tempfile=NULL;
	const char *pos=NULL;
	int retval=0;
	int poshelper=0;
	unsigned long uidcheck=0;

	errno=0;
	if (stat(filename, &attrib)!=0) {               /* get the attributes of filename */
		fprintf(stderr,"[%s]:%d: '%s': %s\n",programmname,__LINE__,filename,strerror(errno));
		return 0;
	}

	if ((tempfile=alloca(sizeof(char)*(strlen(filename)+1))) == NULL) {
		return MEMORY_ERROR;
	} 
	strncpy(tempfile,filename,strlen(filename)+1);
	strcat(tempfile,"\0");
	check_user=NULL;
	if (param != NULL ) {
		for(running=param;(running!=NULL) && (retval==0);running=running->next)
		{
			switch (running->paramtype) {
				case NAME_FLAG: 
					cmp_value=fnmatch(running->param_value, basename(tempfile),0);
					if (cmp_value!=0) {
						retval= NO_FILE_MATCH;
						defaultprint=0;
					}
					break;
				case USER_FLAG:
					if ((check_user= getuser(running->param_value,DONTCHECKUIDFLAG))!=NULL) {
						if(attrib.st_uid!=check_user->pw_uid) {
							retval= NO_FILE_MATCH;
							defaultprint=0;
						}
					} else {
						uidcheck=makeuid(running->param_value);
						if((uidcheck==0) || (attrib.st_uid!=uidcheck)) { /* we now also search for non existing numeric users. Hopefully */
							retval=NO_USER_MATCH;
							defaultprint=0;
							}
						}
					break; 
				case PATH_FLAG: 
					cmp_value=fnmatch(running->param_value, tempfile,0);
					if (cmp_value!=0) {
						retval= NO_FILE_MATCH;
						defaultprint=0;
					}
					break;

				case TYPE_FLAG: 
					pos=running->param_value; 
					switch ((pos[0]<96)?pos[0]+32:pos[0]) {
						case TYPE_FLAG_B:
								if(S_ISBLK(attrib.st_mode)==0) {
									retval= NO_FILE_MATCH;
									defaultprint=0;
								}
								break;
						case TYPE_FLAG_C: 
								if(S_ISCHR(attrib.st_mode)==0) {
									retval= NO_FILE_MATCH;
									defaultprint=0;
								}
								break;
						case TYPE_FLAG_D: 
								if(S_ISDIR(attrib.st_mode)==0) {
									retval= NO_FILE_MATCH;
									defaultprint=0;
								}
								break;
						case TYPE_FLAG_P: 
								if(S_ISFIFO(attrib.st_mode)==0) {
									retval= NO_FILE_MATCH;
									defaultprint=0;
								}
								break;
						case TYPE_FLAG_F: 
								if(S_ISREG(attrib.st_mode)==0) {
									retval= NO_FILE_MATCH;
									defaultprint=0;
								}
								break;
						case TYPE_FLAG_L: 
								if(S_ISLNK(attrib.st_mode)==0) {
									retval= NO_FILE_MATCH;
									defaultprint=0;
								}
								break;
						case TYPE_FLAG_S: 
								if(S_ISSOCK(attrib.st_mode)==0) {
									retval= NO_FILE_MATCH;
									defaultprint=0;
								}
								break;
						}
					break;
				case NOUSER_FLAG: 
			
					if ((check_user= getuser(NULL,attrib.st_uid))!=NULL) {
						retval= NO_FILE_MATCH;
						defaultprint=0;
					}
					
					break;
				case PRINT_FLAG:
				case LS_FLAG:
					if (retval==0) {
						do_output(filename, running, &attrib); /* if we print or ls is done in do_output */
						defaultprint=0;
					}
					break;
				default: 
					/* ### FB: Fehlerbehandlung printf() fehlt */
					printf("dont attach new flags (%d) without changing default here: %d. Thx \n",running->paramtype,__LINE__);
			}

		}
	}
	if (retval==NO_FILE_MATCH) {
		retval=0; 
	}
	if (defaultprint==1) {
		do_output(filename,param,&attrib); /* after we checked all parameters and havent print out anything print atleast once */
	}
	if (S_ISDIR(attrib.st_mode)) {
		poshelper=strlen(filename);
		if ((filename[poshelper-2]!='/') && (filename[poshelper-1]!='.')) {
			if((do_directory(filename,param))==MEMORY_ERROR) {
				return MEMORY_ERROR;
			}
		}
	}
	return retval;

}
/**
 *
 * \brief works on one directory and calls do_file for every directory entry
 *
 * checks if directory is a single file from searchparameter, and if yes directly call do_file
 * otherwise the directory is opened and every directory entry is hand over to do_file
 *
 * \param pathname is a string which includes the current path
 * \param struct params. The start point for the linked list of parameters
 *
 * \return 0 or in case of error 1
 * \retval 0 no error
 * \retval 1 User not found
 * \retval 99 Memory error
 *
 */
int do_directory(const char *pathname, const struct params *param)
{
	DIR *dirp = NULL;
        struct stat attrib;                     /* create a file attribute structure */

	char *fullfile=NULL;

	struct dirent *entryp; 		/* dir entry pointer */
	int return_value=0;

	if (stat(pathname, &attrib)!=0) {               /* get the attributes of filename */
		fprintf(stderr,"[%s]:%d: '%s': %s\n",programmname,__LINE__,pathname,strerror(errno));
		return 0;
	}
	if (S_ISREG(attrib.st_mode)) { 
		if((return_value=do_file(pathname,param))==MEMORY_ERROR) {
			return MEMORY_ERROR;
		}
		return return_value;
	} 

	dirp = opendir(pathname);
	if (dirp==NULL) {
		fprintf(stderr,"[%s]:%d: '%s': %s\n",programmname,__LINE__,pathname,strerror(errno));
		return 0;
	}
	else
	{
		while ((dirp) && (entryp = readdir (dirp)))
		{
			if (strcmp(entryp->d_name,"..")==0)  {
				continue;
			}
			if ((fullfile=malloc(sizeof(char)*(strlen(pathname)+strlen(entryp->d_name)+3)))==NULL) {
				(void) closedir (dirp);
				return MEMORY_ERROR;
			}	
			if (pathname[strlen(pathname)-1]=='/') {
				sprintf(fullfile,"%s%s%c",pathname, entryp->d_name, '\0');
			} else { 
				sprintf(fullfile,"%s/%s%c",pathname, entryp->d_name, '\0');
			}
			return_value=do_file((const char *) fullfile, param);
			if (return_value==MEMORY_ERROR) {
				(void) free(fullfile);
				(void) closedir (dirp);
				(void) free(fullfile);
				return MEMORY_ERROR;
			}
			if (fullfile!=NULL) {
				free(fullfile);
				fullfile=NULL;
			}
		}
		(void) closedir (dirp);
		dirp=NULL;
	}

	return return_value;

}
/**
 *
 * \brief double code put into function to to strtol
 *
 *
 * \param username is a string for the username
 *
 * \return long or 0
 */
unsigned long makeuid(const char *username)
{
	long uidcheck=0;
	char *pos=NULL;

	uidcheck=strtol(username,&pos,10);


	return (pos[0]=='\0') ? uidcheck : 0;

}
/**
 *
 * \brief checks the /etc/passwd for the given uid or username
 *
 * if username is NULL and uid is given, directly checks for the uid in passwd file, used to get user for a file for output
 * otherwise the passwd file is searched for the username and if username is not found, it search for converted uid of given username
 *
 * \param username is a string for the username
 * \param long fileuid is the uid, mostly directly read from a file
 *
 * \return NULL or passwd structure of found user.
 * \retval NULL in case of error
 * \retval struct passwd* if a user was found.
 *
 */
struct passwd* getuser(const char *username, const long fileuid) 
{
	long uidcheck=0;

	struct passwd *local_user=NULL;
	/* check user against passwd file to get uid.
	 * regarding man page a -user parameter is always handled as name. 
	 * If username is not found in passwd file then parameter is handled as UID parameter directly.
	 */		

	if ((username == NULL) && (fileuid != DONTCHECKUIDFLAG)) { /* used for nouser flag */
		if ((local_user = getpwuid(fileuid))!=NULL) {
			return local_user;
		} else {
			return NULL; /* correct value for nouser flag */
		}
	} else {	
		uidcheck=makeuid(username);


		if ((local_user = getpwnam(username))!=NULL) {
			return local_user;
		} else if ((uidcheck!=0) && ((local_user = getpwuid(uidcheck))!=NULL)) {
			/* we found a user either with name match or with strtol converted UID */
			return local_user;
		} else {
			return NULL;	
			/* strtol converted completly string to long AND check value against passwd UID dont work*/
			/* user not found (neither name nor uid)*/
				
		}
	}
	return NULL;
}
/**
 *
 * \brief checks the /etc/groups for the given gid or groupname
 *
 * if groupname is NULL and gid is given, directly checks for the gid in groups file, used to get group for a file for output
 * otherwise the groups file is searched for the groupname and if groupname is not found, it search for converted gid of given groupname
 *
 * \param groupname is a string for the groupname
 * \param long grpuid is the gid, mostly directly read from a file
 *
 * \return NULL or group structure of found group.
 * \retval NULL in case of error
 * \retval struct group* if a group was found.
 *
 */
struct group* getgroup(const char *groupname, const long grpuid) 
{
	long uidcheck=0;
	char *pos=NULL;

	struct group *local_group=NULL;

	if ((groupname == NULL) && (grpuid != DONTCHECKUIDFLAG)) { /* used for nogroup flag */
		if ((local_group = getgrgid(grpuid))!=NULL) {
			return local_group;
		} else {
			return NULL; /* correct value for nogroup flag */
		}
	} else {	
		uidcheck=strtol(groupname,&pos,10);


		if ((local_group = getgrnam(groupname))!=NULL) {
			return local_group;
		} else if ((pos[0]=='\0') && ((local_group = getgrgid(uidcheck))!=NULL)) {
			/* we found a group either with name match or with strtol converted GID */
			return local_group;
		} else {
			/* strtol converted completly string to long AND check value against groups GID dont work*/
			/* group not found (neither name nor uid)*/
				
		}
	}
	return NULL;
}
/**
 *
 * \brief prints out the values of a file
 *
 * with the given option (print or ls) the output is formated
 *
 * \param fullfilename is a string for the full path including filename
 * \param params is the actual parameter structure to see if we do print or ls
 * \param file_stat is the stat value of a file
 *
 * \return 0
 * \retval 0
 *
 */
int do_output(const char *fullfilename, const struct params *param, const struct stat *file_stat)
{
	struct passwd *local_user=NULL;
	struct group *local_group=NULL;
	unsigned int uid=0,gid=0;

	if ((param!=NULL) && ((param->paramtype & LS_FLAG)!=0)) {
		/* ### FB: Fehlerbehandlung printf() fehlt */
		printf("%6lu\t", file_stat->st_ino);
		printf("%4lu ", (file_stat->st_blocks /2 ));
		
		get_file_attribs(file_stat);

		/* ### FB: Fehlerbehandlung printf() fehlt */
		printf("\t%d", (file_stat->st_nlink!=0)?file_stat->st_nlink:0);
		uid = file_stat->st_uid;
		local_user = getuser(NULL,uid);
		if ((local_user != NULL) && (local_user->pw_uid == uid)) {
		/* ### FB: Fehlerbehandlung printf() fehlt */
			printf(" %s\t", local_user->pw_name);
		} else {
			printf(" %d\t", uid);
		}
		gid = file_stat->st_gid;
		local_group = getgroup(NULL,gid);
		if ((local_group != NULL) && (local_group->gr_gid == gid)) {
		/* ### FB: Fehlerbehandlung printf() fehlt */
			printf(" %s\t", local_group->gr_name);
		} else {
			printf(" %d\t", uid);
		}
		
		/* ### FB: Fehlerbehandlung printf() fehlt */
		printf("%10ld ", file_stat->st_size);
		gettime(file_stat->st_mtime);
	}	
	/* ### FB: Fehlerbehandlung printf() fehlt */
	printf("%s \n",fullfilename);	
	return 0;
}
/**
 * \brief get and converts the last access time for a file into a readable format
 *
 * \param file_stat is the stat structure for a file
 */
void gettime(const time_t timep) 
{
	struct tm *tm;
	char datastring[256];
	char *time_fmt = NULL;

	
	if (time_fmt == NULL )
	{
		time_fmt = "%b  %e %R";
	}
	tm = localtime(&timep);
	
	strftime(datastring, sizeof(datastring), time_fmt, tm);
	/* ### FB: Fehlerbehandlung printf() fehlt */
	printf("%s ", datastring);

}
/**
 * \brief get and print all rights and file attributes if they match
 *
 *
 * \param file_stat is the stat structure for a file
 */
void get_file_attribs(const struct stat *file_stat)
{

	/* ### FB: Fehlerbehandlung printf() fehlt ein paar Mal ;) */
	if(S_ISDIR(file_stat->st_mode)) {
		printf("%c",'d');
	} else if(S_ISCHR(file_stat->st_mode)) {
		printf("%c",'c');
	} else if(S_ISBLK(file_stat->st_mode)) {
		printf("%c",'b');
	} else if(S_ISFIFO(file_stat->st_mode)) {
		printf("%c",'p');
	} else if(S_ISLNK(file_stat->st_mode)) {
		printf("%c",'l');
	} else if(S_ISSOCK(file_stat->st_mode)) {
		printf("%c",'s');
	} else {
		printf("%c",'-');
	}
	
	printf("%c",(file_stat->st_mode & S_IRUSR)?'r':'-'); 
	printf("%c",(file_stat->st_mode & S_IWUSR)?'w':'-');
	if((file_stat->st_mode & S_ISUID))
	{
		printf("%c",(file_stat->st_mode&S_IXUSR)?'s':'S');
	}
	else if((file_stat->st_mode & S_IXUSR))
	{
		printf("%c",(file_stat->st_mode & S_IXUSR)?'x':'-');
	}
	else
	{
		printf("%c",'-');
	}
	printf("%c",(file_stat->st_mode & S_IRGRP)?'r':'-');
	printf("%c",(file_stat->st_mode & S_IWGRP)?'w':'-');
	if((file_stat->st_mode & S_ISGID))
	{
		printf("%c",(file_stat->st_mode & S_IXGRP)?'s':'S');
	}
	else if((file_stat->st_mode & S_IXGRP))
	{
		printf("%c",(file_stat->st_mode & S_IXGRP)?'x':'-');
	}
	else
	{
		printf("%c",'-');
	}
	printf("%c",(file_stat->st_mode & S_IROTH)?'r':'-');
	printf("%c",(file_stat->st_mode & S_IWOTH)?'w':'-');
	if((file_stat->st_mode & S_ISVTX))
		{
			printf("%c",(file_stat->st_mode & S_IXOTH)?'t':'T');
		}
	else if((file_stat->st_mode & S_IXOTH))
	{
		printf("%c",(file_stat->st_mode & S_IXOTH)?'x':'-');
	}
	else
	{
		printf("%c",'-');
	}
}
/*! postusage value is where error occurs: 
 * 1 user
 * 2 name
 * 3 path
 * 4 type
 * 5 unknown parameter
 */
/**
 *
 * \brief prints out the usage of myfind
 *
 * \param postusage:
 *		   1 user
 *		   2 name
 *		   3 path
 *		   4 type
 *		   5 wrong parameter
 */
void do_usageprint(int postusage) 
{

	postusage=postusage;
	switch(postusage) {
		case 1:
			fprintf(stderr,"[%s]\t: -user benoetigt einen parameter: [username] oder [uid].\n",programmname); 
			break;
		case 2:
			fprintf(stderr,"[%s]\t: -name benoetigt einen parameter: [PATTERN].\n",programmname);
			break;
		case 3:
			fprintf(stderr,"[%s]\t: -path benoetigt einen parameter: [PATTERN].\n",programmname);
			break;
		case 4:
			fprintf(stderr,"[%s]\t: -type benoetigt einen parameter: [bcdpfls].\n",programmname); 
			break;
		case 5:
			fprintf(stderr,"[%s]\t: unbekannter parameter erkannt.\n",programmname);
		default:
			/* ### FB: Fehlerbehandlung printf() fehlt */
			printf("Usage: myfind [path...] [expressions]\n\n");
			printf("expressions:\n\t-name PATTERN\n\t-user username or UID\n\t-path PATTERN\n\t-type [bcdpfls]\n\t-nouser\n\t-print\n\t-ls\n\n");
			break;
	}

	
}
/*
 * =================================================================== eof ==
 */
