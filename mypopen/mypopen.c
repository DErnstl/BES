#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main (int argc, const char *argv[])
{
	int pipefd[2];
	static pid_t pid;
	char buf;
	int status;

	if (pipe(pipefd) == -1) {
		/* Error handling: pipe error */
		/* errno is set automatically */
		exit(EXIT_FAILURE);
	}
    
	pid = fork();
	if (pid == -1) {
		/* Error handling: fork error */
		/* errno is set automatically */
		exit(EXIT_FAILURE);
	}

	/* Case: Child reads, parent writes */
	if (pid == 0) {				/* child */
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execl("/bin/sh", argv[1]);
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	} else {				/* parent */
		close(pipefd[1]);
		while (read(pipefd[0], &buf, 1) > 0) {
			write(STDOUT_FILENO, &buf, 1);		
		}
		close(pipefd[0]);
		waitpid(pid, &status, 0);
		exit(EXIT_SUCCESS);
	}
}
