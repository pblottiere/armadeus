#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	char* str = 0;
	int shmid = shm_open("/armadeus-shm", O_RDONLY, S_IRWXU);

	if (shmid < 0) {
		perror("Cannot open shared memory object");
		
		return EXIT_FAILURE;
	}

	if ((str = mmap(0, 1024*1024, PROT_READ, MAP_SHARED, shmid, 0)) == MAP_FAILED) {
		perror("Cannot map shared memory object");
		close(shmid);

		return EXIT_FAILURE;
	}

	printf("shmwriter wrote:\n");
	printf("%s", str);
	printf("Now make some changes in the shared memory object (/dev/shm/armadeus-shm) then press RETURN...\n");
	printf("For example, on a second console you can run 'echo foo bar > /dev/shm/armadeus-shm'\n");

	getchar();

	printf("you wrote:\n");
	printf("%s", str);

	munmap(str, 1024*1024);
	close(shmid);

	return EXIT_SUCCESS;
}
