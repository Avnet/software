
/*
 * Modified for the Maxim Integrated MAX44000 Sensor
 * Copyright (c) 2016 Avnet, Inc.  All rights reserved.
 *
 * Avnet, Inc.
 * Avnet IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, Avnet IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * Avnet EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdint.h>

#include <linux/input.h>

#define SYS_IIO_PATH	"/sys/bus/iio/devices"
#define SENSOR_NAME 	"max44000"

#define DEV_IIO_FILE	"/dev/iio:device0"

#define BUF_SIZE 2
#define IN 0
#define OUT 1

#define PWM_ADDR 0x80010000

#define TRIGGER_NAME "sysfstrig1"


volatile int interrupt = 0;


// Handle signal interrupt
void sigHandler(int signo) {
	printf("SigINT received.\n");
	interrupt = 1;
}

/*
void write_pwm(int pwm_value)
{
	int fd;
	//unsigned pwm_addr = 0x80010000;
	
	unsigned page_addr, page_offset;
	void *ptr;
	unsigned page_size=sysconf(_SC_PAGESIZE);

    if((fd = open("/dev/mem", O_RDWR)) == -1) {
		perror("opening pwm device");
		exit(EXIT_FAILURE);
	}
	
	// mmap the device into memory
	page_addr = (PWM_ADDR & (~(page_size-1)));
	page_offset = PWM_ADDR - page_addr;	
	ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr);

	// Write to PWM
	*((unsigned *)(ptr + page_offset)) = pwm_value *10000;

	munmap(ptr, page_size);
	(void)close(fd);
}
*/


int main()
{
	int fd;
	int dev_fd;
	int trig_fd;
	int proximity;
	char buf[2];
	char *buf_len = "100";

	int mem_fd;
	//unsigned pwm_addr = 0x80010000;
	
	unsigned page_addr, page_offset;
	void *ptr;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	char on = '1';
	char off = '0';

	//catch interrupt signal
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	printf("Starting the MAX44000 Proximity Sensor Demo App.\n");

	// Add the trigger
    if((fd = open("/sys/bus/iio/devices/iio_sysfs_trigger/add_trigger", O_WRONLY)) == -1) {
		perror("opening "SENSOR_NAME" device add_trigger file");
		exit(EXIT_FAILURE);
	}
	write(fd, &on, 1);
	(void)close(fd);

/*
	// DEBUG: Make sure the trigger was added by checking for the trigger0 file
	if (access("/sys/bus/iio/devices/trigger0", F_OK) != -1) {
		printf("/sys/bus/iio/devices/trigger0 file exists \n");
	}
	else {
		perror(" /sys/bus/iio/devices/trigger0 file");
		exit(EXIT_FAILURE);
	}		
*/
	// Connect the trigger to the device
    if((fd = open("/sys/bus/iio/devices/iio:device0/trigger/current_trigger", O_WRONLY)) == -1) {
		perror("opening "SENSOR_NAME" device current_trigger file");
		exit(EXIT_FAILURE);
	}
	write(fd, TRIGGER_NAME, sizeof(TRIGGER_NAME));
	(void)close(fd);


	// Set the buffer length 
    if((fd = open("/sys/bus/iio/devices/iio:device0/buffer/length", O_WRONLY)) == -1) {
		perror("opening "SENSOR_NAME" device buffer length file");
		exit(EXIT_FAILURE);
	}
	write(fd, &buf_len, sizeof(buf_len));
	(void)close(fd);


	// Enable the proximity input channel
    if((fd = open("/sys/bus/iio/devices/iio:device0/scan_elements/in_proximity_en", O_WRONLY)) == -1) {
		perror("opening "SENSOR_NAME" device proximity_en file");
		exit(EXIT_FAILURE);
	}
	write(fd, &on, sizeof(on));
	(void)close(fd);


	// Enable the buffer
    if((fd = open("/sys/bus/iio/devices/iio:device0/buffer/enable", O_WRONLY)) == -1) {
		perror("opening "SENSOR_NAME" device buffer enable file");
		exit(EXIT_FAILURE);
	}
	write(fd, &on, sizeof(on));
	(void)close(fd);

	// Open the device file
    if((dev_fd = open(DEV_IIO_FILE, O_RDONLY)) == -1) {
		perror("opening "SENSOR_NAME" device file");
		exit(EXIT_FAILURE);
	}

	// Open the trigger file
    if((trig_fd = open("/sys/bus/iio/devices/trigger0/trigger_now", O_WRONLY)) == -1) {
		perror("opening "SENSOR_NAME" device trigger_now file");
		exit(EXIT_FAILURE);
	}

	// Open the /dev/mem file for the PWM
    if((mem_fd = open("/dev/mem", O_RDWR)) == -1) {
		perror("opening pwm device");
		exit(EXIT_FAILURE);
	}
	

	while(!interrupt) {
		// Pull the trigger
		write(trig_fd, &on, sizeof(on));

		// Read the iio device file
		if (read(dev_fd, buf, sizeof(buf)) >= 0) {
			proximity = (buf[0]);
		 	//printf("Proximity = %d\n", proximity);
			//write_pwm(proximity);
		}
		else {
			printf("Sensor read failed...aborting\n");
			break;
		}

		// mmap the device into memory
		page_addr = (PWM_ADDR & (~(page_size-1)));
		page_offset = PWM_ADDR - page_addr;	
		ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, page_addr);

		// Write to PWM
		*((unsigned *)(ptr + page_offset)) = proximity *10000;
		
		// unmap the memory
		munmap(ptr, page_size);

		//sleep(1);
		//usleep(200000); // sleep 200ms
	}
	
	// Close the /dev/mem file and unmap
	(void)close(mem_fd);

	// Close the trigger file
	(void)close(trig_fd);

	// Close the device file
	printf("Closing the device file...\n");
	(void) close(dev_fd);

	// Disable the buffer
	printf("Disabling and closing the device buffer...\n");
    if((fd = open("/sys/bus/iio/devices/iio:device0/buffer/enable", O_WRONLY)) == -1) {
		perror("opening "SENSOR_NAME" device buffer enable file");
		exit(EXIT_FAILURE);
	}
	write(fd, &off, sizeof(off));
	(void)close(fd);

	//printf("Turn off the LEDs...\n");
	//write_pwm(0);

	printf("Goodbye!\n");

	return 0;
}

