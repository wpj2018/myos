#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void test_name_pipe_write()
{
	int fd = open("/tmp/fifo", O_WRONLY);
	char buf[50];
	sprintf(&buf[0], "writer pid = %d\n", getpid());
	write(fd, buf, sizeof(buf));
}

int main(void)
{
	test_name_pipe_write();
	return 0;
}
