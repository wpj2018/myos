#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void test_name_pipe_read()
{
	if (access("/tmp/fifo", 0644) != 0) {
		mkfifo("/tmp/fifo", 0644);
	}
	int fd = open("/tmp/fifo", O_RDONLY);
	char buf[50];
	read(fd, buf, sizeof(buf));
	printf("reader recv msg: %s", buf);
}

int main(void)
{
	test_name_pipe_read();
	return 0;
}
