/* 
 * Generates a square wave on platform dependant GPIO.
 * taken from: https://rt.wiki.kernel.org/articles/s/q/u/Squarewave-example.html
 *
 * compile using  "arm-linux-gcc -o swave swave.c -lrt -Wall"
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/mman.h>	/* mmap */

#define NSEC_PER_SEC    1000000000
#define MAP_SIZE 4096
#define MAP_MASK ( MAP_SIZE - 1 )
#define u32	unsigned int

/* Platform specific stuff (here for APF51) */
#define GPIO_PIN	2
#define GPIO_DR_ADDR	0x73f84000
#define GPIO_DR_MASK	(~(1 << GPIO_PIN))

/* using clock_nanosleep of librt */
extern int clock_nanosleep(clockid_t __clock_id, int __flags,
				__const struct timespec *__req,
				struct timespec *__rem);

/* the struct timespec consists of nanoseconds and seconds. if the
 * nanoseconds are getting bigger than 1000000000 (= 1 second) the
 * variable containing seconds has to be incremented and the
 * nanoseconds decremented by 1000000000.
 */
static inline void tsnorm(struct timespec *ts)
{
	while (ts->tv_nsec >= NSEC_PER_SEC) {
		ts->tv_nsec -= NSEC_PER_SEC;
		ts->tv_sec++;
	}
}

void out()
{
	static unsigned char state=0;
	static int fd = -1;
	static void *map, *regaddr;
	u32 addr = GPIO_DR_ADDR;

	if (fd == -1) {
		fd = open("/dev/mem", O_RDWR | O_SYNC);
		if (fd < 0) {
			perror("open(\"/dev/mem\")");
			exit(1);
		}
		map = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
				fd, addr & ~MAP_MASK);
		if (map == (void *)-1) {
			perror("mmap()");
			exit(1);
		}
		regaddr = map + (addr & MAP_MASK);
	}

	state = state ? 0 : 1;
	*(u32 *) regaddr = (*(u32 *) regaddr & GPIO_DR_MASK) | (state << GPIO_PIN);
}

int main(int argc, char** argv)
{
	struct timespec t;
	struct sched_param param;

	/* default interval = 100000000ns = 100ms */
	int interval = 100000000;

	if(argc >= 2 && atoi(argv[1]) > 0) {
		printf("using realtime, priority: %d\n",atoi(argv[1]));
		param.sched_priority = atoi(argv[1]);
		/* enable realtime fifo scheduling */
		if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
			perror("sched_setscheduler failed");
			exit(-1);
		}
	}
	if (argc >= 3)
		interval = atoi(argv[2]);

	/* get current time */
	clock_gettime(0, &t);
	/* start after one second */
	t.tv_sec++;
	while(1) {
		/* wait until next shot */
		clock_nanosleep(0, TIMER_ABSTIME, &t, NULL);
		/* do the stuff */
		out();
		/* calculate next shot */
		t.tv_nsec += interval;
		tsnorm(&t);
	}

	return 0;
}

