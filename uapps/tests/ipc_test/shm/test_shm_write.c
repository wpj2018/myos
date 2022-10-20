#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int SHM_SZ = 4096;

void test_shm_write()
{
	key_t shm_key = 0x666;
	int shm_id;
	char *shm_p;

	shm_id = shmget(shm_key, SHM_SZ, 0644 | IPC_CREAT);

	shm_p = (char *)shmat(shm_id, NULL, 0);
	sprintf(shm_p, "writer pid = %d\n", getpid());

	shmdt(shm_p);
}

int main(void)
{
	test_shm_write();
	return 0;
}
