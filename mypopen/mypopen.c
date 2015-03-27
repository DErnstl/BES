#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "mypopen.h"

FILE *mypopen(const char *cmd, const char *type)
{
	int file_fd[2];
	pid_t pid;
	FILE *file_stream;

	/* only allow "r" or "w" */
	if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
		errno = EINVAL;
		return(NULL);
	}

	if (limit > 1) {
		errno = ESTRPIPE;
		return(NULL);
	}

	/* Pipe error */
	if (pipe(file_fd) == -1) {
		return(NULL);	/* errno set by pipe() */
	} else {
		limit++;
	}

	if ((pid = fork()) == -1) {
		/* errno set by fork() */
		close(file_fd[0]);
		close(file_fd[1]);
		return(NULL);
	} else if (pid == 0) {	/* child */
		if (*type == 'r') {
			close(file_fd[0]);
			dup2(file_fd[1], STDOUT_FILENO);
			close(file_fd[1]);
		} else {
			close(file_fd[1]);
			dup2(file_fd[0], STDIN_FILENO);
			close(file_fd[0]);
		}

		/* Error handling??? */
		execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);

		if (*type == 'r') {
			close(STDOUT_FILENO);
		} else {
			close(STDIN_FILENO);
		}
	} else { /* parent  */
		if (*type == 'r') {
			close(file_fd[1]);
			if ((file_stream = fdopen(file_fd[0], type)) == NULL) {
				return(NULL);
			}
		} else {
			close(file_fd[0]);
			if ((file_stream = fdopen(file_fd[1], type)) == NULL) {
				return(NULL);
			}
		}

		child_pid = pid;		/* remember child_pid for this fd */
		return(file_stream);
	}
}

int mypclose(FILE *file_stream)
{
	int fd;
	int stat;

	if (child_pid == 0) {
		errno = EINVAL;
		return(-1);
	}

	if (fclose(file_stream) == EOF) {
		return(-1);
	}

	while (waitpid(child_pid, &stat, 0) == -1) {
		if (errno != EINTR) {
			return(-1);	/* error other than EINTR from waitpid() */
		}
	}

	child_pid = 0;

	return(stat);
}

