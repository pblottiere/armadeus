#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>

int main(int argc, char *argv[])
{
	struct utsname sysnames;
	char* str = "Hello ! You are on ";
	int shmid = shm_open("/armadeus-shm", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);

	if (shmid < 0) {
		perror("Cannot open shared memory segment.");

		return EXIT_FAILURE;
	}

	if (ftruncate(shmid, 1024*1024) < 0) {
		perror("Cannot resize shared memory segment.");
		close(shmid);
		shm_unlink("/armadeus-shm");

		return EXIT_FAILURE;
	}

	write(shmid, str, sizeof(char)*strlen(str));
	
	uname(&sysnames);

	write(shmid, sysnames.nodename, sizeof(char)*strlen(sysnames.nodename));
	write(shmid, ".\n", 2*sizeof(char));

	close(shmid);

	return EXIT_SUCCESS;
}
