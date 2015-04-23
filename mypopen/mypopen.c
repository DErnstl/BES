/**
 * @file mypopen.o
 * Betriebssysteme mypopen Library
 * Beispiel 2
 *
 * @author Adam Kerenyi <ic14b080@technikum-wien.at>
 * @author Romeo Beck <ic14b037@technikum-wien.at>
 * @author Thomas Zeitinger <ic14b033@technikum-wien.at>
 * @date 2015/04/24
 *
 * @version 470 
 *
 */


/*
 * -------------------------------------------------------------- includes --
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "mypopen.h"

/*
 * --------------------------------------------------------------- defines --
 */

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */

static pid_t child_pid = -1;
static unsigned int limit = 0;
static FILE *file_compare = NULL;

/*
 * ------------------------------------------------------------- functions --
 */

/**
 *
 * \brief mypopen - popen clone
 *
 * Library to simulate popen
 *
 * \param cmd, type
 *
 * \return Filestream
 * \retval 0
 *
 */

FILE *mypopen(const char *cmd, const char *type)
{
	int file_fd[2];
	pid_t pid;
	FILE *file_stream;

	if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
		errno = EINVAL;
		return(NULL);
	}

	/* only one instance is allowed */
	if (limit > 0) {
		errno = EAGAIN;
		return(NULL);
	}

	if (pipe(file_fd) == -1) {
		return(NULL);
	}

	if ((pid = fork()) == -1) {
		close(file_fd[0]);
		close(file_fd[1]);
		return(NULL);
	} else if (pid == 0) {	/* child */
		if (*type == 'r') {
			close(file_fd[0]);
			if (dup2(file_fd[1], STDOUT_FILENO) == -1) {
				close(file_fd[1]);
				exit(EXIT_FAILURE);
			}
			close(file_fd[1]);
		} else {
			close(file_fd[1]);
			if (dup2(file_fd[0], STDIN_FILENO) == -1) {
				close(file_fd[1]);
				exit(EXIT_FAILURE);
			}
			close(file_fd[0]);
		}

		if (execl("/bin/sh", "sh", "-c", cmd, (char *)NULL) == -1) {
			if (*type == 'r') {
				close(STDOUT_FILENO);
			} else {
				close(STDIN_FILENO);
			}
			return(NULL);
		}

		if (*type == 'r') {
			close(STDOUT_FILENO);
		} else {
			close(STDIN_FILENO);
		}
	} else { /* parent  */
		if (*type == 'r') {
			close(file_fd[1]);
			if ((file_stream = fdopen(file_fd[0], type)) == NULL) {
				close(file_fd[0]);
				return(NULL);
			}
		} else {
			close(file_fd[0]);
			if ((file_stream = fdopen(file_fd[1], type)) == NULL) {
				close(file_fd[1]);
				return(NULL);
			}
		}

		child_pid = pid;
		file_compare = file_stream;
		limit++;
		return(file_stream);
	}

	return(NULL);
}

/**
 *
 * \brief myclose - plose clone
 *
 * Library to simulate pclose
 *
 * \param file_stream
 *
 * \retval Exitstatus child or -1
 *
 */

int mypclose(FILE *file_stream)
{
	int stat;

	if (limit == 0) {
		errno = ECHILD;
		return(-1);
	}

	if (child_pid == -1) {
		errno = EINVAL;
		return(-1);
	}

	if (file_compare != file_stream) {
		errno = EINVAL;
		return(-1);
	}

	if (fclose(file_stream) == EOF) {
		errno = EINVAL;
		return(-1);
	}

	while (waitpid(child_pid, &stat, 0) == -1) {

		if (errno != EINTR) {
			return(-1);
		}
	}

	child_pid = -1;

	if (WIFEXITED(stat)) {
		return (WEXITSTATUS(stat));
	}

	errno = ECHILD;
	return(-1);
}


/*
 * =================================================================== eof ==
 */
