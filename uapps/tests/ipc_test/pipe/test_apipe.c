#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void test_anony_pipe()
{
	int pipefd[2];
	int pid;
	char buf[64];

	pipe(pipefd);
	pid = fork();
	if (pid == 0) { // read pipe
		close(pipefd[1]); // close write pipe end

		read(pipefd[0], buf, sizeof(buf));

		printf("child pid = %d, recv msg: %s", getpid(), buf);
	} else if (pid > 0) { // write pipe
		close(pipefd[0]); // close read pipe end

		char msg[50];
		sprintf(&msg[0], "parent pid = %d\n", getpid());
		write(pipefd[1], msg, strlen(msg));

		wait(NULL);
	} else {
	}
}

int main(void)
{
	test_anony_pipe();
	return 0;
}
