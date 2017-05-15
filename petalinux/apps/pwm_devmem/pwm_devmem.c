/*
 * This application reads/writes GPIO devices with /dev/mem.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

#define IN 0
#define OUT 1

void usage(void)
{
	printf("*argv[0] -p <PWM_ADDRESS> -o <VALUE>\n");
	printf("    -p               PWM physical address");
	printf("    -o <VALUE>       Output to PWM\n");
	return;
}

int main(int argc, char *argv[])
{
	int c;
	int fd;
	int direction=IN;
	unsigned pwm_addr = 0;
	int value = 0;
	
	unsigned page_addr, page_offset;
	void *ptr;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	printf("PWM access through /dev/mem.\n");

	while((c = getopt(argc, argv, "p:o:h")) != -1) {
		switch(c) {
		case 'p':
			pwm_addr=strtoul(optarg,NULL, 0);
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

	if (pwm_addr == 0) {
		printf("PWM physical address is required.\n");
		usage();
		return -1;
	}
	
	/* Open /dev/mem file */
	fd = open ("/dev/mem", O_RDWR);
	
	if (fd < 1) {
		perror(argv[0]);
		return -1;
	}
	
	/* mmap the device into memory */
	page_addr = (pwm_addr & (~(page_size-1)));
	page_offset = pwm_addr - page_addr;	
	ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr);

	/* Write to PWM */

	*((unsigned *)(ptr + page_offset)) = value;

	munmap(ptr, page_size);

	return 0;
}



