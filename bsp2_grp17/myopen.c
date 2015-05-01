/* ### FB: doxygen header and separators missing */

/* ### FB: defines should come after includes*/
/* ### FB: Unsure about this, but our version works without this define and uses fdopen too */
#define _XOPEN_SOURCE 600 /* can't use fdopen otherwise */
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "myopen.h"

static FILE *instance = NULL;
static int processId = -1;

/*
 * \brief executes the given command in a new process and returns a Stream to the output/input of it
 * 
 * \param command the command to be executed in a new process
 * \param type decide whether you want the output 'r' or input 'w' pipe of the given process 
 *
 * \return returns the output or input pipe of the newly executed process, returns NULL on error
 */
extern FILE *mypopen(const char *command, const char *type)
{
	int ioPipes[2];

	if((*type != 'r' && *type != 'w') || strlen(type) != 1) /* only allows type "r" and "w" through */
	{
		errno = EINVAL; /* Invalid argument */
		return NULL;
	}

	if(instance != NULL)
	{
		errno = EAGAIN; /* resource temporarily unavailable */
		return NULL;
	}

	/* ### FB: Suggestion: always use curly braces, even for one line of code */
	if(pipe(ioPipes) == -1) /* pipe(...) does error setting */
		return NULL;

	processId = fork(); /* forks current process */

	if(processId == -1) /* fork error */
	{
		close(ioPipes[0]);
		close(ioPipes[1]);
		return NULL;
	}

	if(processId == 0) /* child process*/
	{
		if(*type == 'r')
		{
			close(ioPipes[0]); /* close pipe we don't need if type is r */
			if(dup2(ioPipes[1], 1) == -1 ) /* fileno(stdout) */
			{
				close(ioPipes[1]); /* close pipe on error */
				exit(EXIT_FAILURE);
			}
			close(ioPipes[1]);
		}
		else /* must be 'w' */
		{
			close(ioPipes[1]); /* close pipe we don't need if type is w */
			if(dup2(ioPipes[0], 0) == -1 )/* fileno(stdin) */
			{
				close(ioPipes[0]); /* close pipe on error */
				exit(EXIT_FAILURE);
			}
			close(ioPipes[0]);

		}
		/* ### FB: typecast missing: should be (char *) NULL */
		execl("/bin/sh", "sh", "-c", command, NULL); /* execute given command */
		/* ### FB: missing error handling for execl */
		exit(3); /* 3 = normal process termination */
	}
	else /* parent process*/
	{
		if(*type == 'r')
		{
			close(ioPipes[1]);  /* close pipe we don't need if type is r */
			if((instance = fdopen(ioPipes[0], type)) == NULL) /* statically saves read pipe */
			{
				close(ioPipes[0]); /* close pipe on error */
				return NULL;
			}
		}
		else /* must be 'w' */
		{
			close(ioPipes[0]);  /* close pipe we don't need if type is w */
			if((instance = fdopen(ioPipes[1], type)) == NULL) /* statically saves write pipe */
			{
				close(ioPipes[1]); /* close pipe on error */
				return NULL;
			}
		}
		/* ### FB: file_compare??? pid??? */
		return instance;
	}
}

/*
 * \brief executes the given command in a new process and returns a Stream to the output/input of it
 * 
 * \stream the Stream to be closed
 *
 * \return returns -1 on error, otherwise the exit code of the child process is returned
 */
extern int mypclose(FILE *stream)
{
	int waitpidReturnVal;
	int waitpidStatus;

	if(processId == -1 && instance == NULL) /* mypclose called before mypopen was called */
	{
		errno = ECHILD; /* There are no child processes. */
		return -1;
	}

	if(stream == NULL || stream != instance) /* NULL argument or wrong stream given */
	{
		errno = EINVAL; /* Invalid argument */
		return -1;
	}

	/* ### FB: Missing error handling for EOF */
	fclose(stream); /* close stream */

	/* waiting for process to die */
	while ((waitpidReturnVal = waitpid(processId, &waitpidStatus, 0)) != processId)
	{
		if(waitpidReturnVal == -1)
		{
			if(errno == EINTR) /* meaningless interruption of syscall waitpid*/
				continue; /* keep waiting */

			errno = ECHILD; /* There are no child processes. */
			return -1;
		}
	}

	if(WIFEXITED(waitpidStatus)) /* returns a value != 0 if status says the process exited normally */
		return WEXITSTATUS(waitpidStatus); /* returns exit return value of dead child process */

	errno = ECHILD;
	return -1;
}
