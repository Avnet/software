/*
 * This application reads/writes GPIO devices with UIO.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#define MAP_SIZE 0x1000
#define PWM_CHANNELS 1
#define PWM_DATA_OFFSET 0x0

#define IN 0
#define OUT 1

void usage(void)
{
	printf("*argv[0] -d <UIO_DEV_FILE> -o <VALUE>\n");
	printf("    -d               UIO device file. e.g. /dev/uio0");
	printf("    -o <VALUE>       Output to PWM\n");
	return;
}

int main(int argc, char *argv[])
{
	int c;
	int fd;
	int direction=IN;
	char *uiod;
	int value = 0;
	int set_flag_d = 0;
	
	void *ptr;

	printf("PWM UIO test.\n");
	while((c = getopt(argc, argv, "d:io:h")) != -1) {
		switch(c) {
		case 'd':
			uiod=optarg;
			set_flag_d = 1;
			break;
		case 'o':
			direction=OUT;
			value=atoi(optarg);
			break;
		case 'h':
			usage();
			return 0;
		default:
			printf("invalid option: %c\n", (char)c);
			usage();
			return -1;
		}
		
	}

	/* Open the UIO device file */
	if (set_flag_d == 1) {
		set_flag_d = 0;
		fd = open(uiod, O_RDWR);
		if (fd < 1) {
			perror(argv[0]);
			printf("Invalid UIO device file:%s.\n", uiod);
			usage();
			return -1;
		}
	}

	/* mmap the UIO device */
	ptr = mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if (ptr == 0) {
		printf("Write PWM: Failed to mmap the PWM UIO device.\n");
		goto failed4;
	}

	/* Write to PWM */

	*((unsigned *)(ptr + PWM_DATA_OFFSET)) = value;


	munmap(ptr, MAP_SIZE);

failed4:
	close(fd);	
	return 0;
}



