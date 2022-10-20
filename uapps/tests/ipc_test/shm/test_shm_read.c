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

void test_shm_read()
{
	key_t shm_key = 0x666;
	int shm_id;
	char *shm_p;

	shm_id = shmget(shm_key, SHM_SZ, 0644);

	shm_p = (char *)shmat(shm_id, NULL, 0);
	printf("reader recv msg:%s", shm_p);

	shmdt(shm_p);
	shmctl(shm_id, IPC_RMID, 0);
}

int main(void)
{
	test_shm_read();
	return 0;
}
