
/*
 * Modified for the ST Microelectronics LIS3MDL Sensor
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
#include <math.h>
#include <string.h>
#include <errno.h>
#include <linux/input.h>

#define OUT_OF_RANGE 	-500.0
#define PROC_DEV		"/proc/bus/input/devices"
#define SENSOR_NAME 	"lis3mdl_mag"

//#define ON				1
//#define OFF				0

volatile int interrupt = 0;

void toggle_mag_sensor(char on_off) {
	int mag_fd;
	int buf_size = 1;
	
	if ((mag_fd = open("/sys/bus/i2c/devices/1-001e/enable_device", O_WRONLY)) == -1) {
		if(errno != ENOENT) {
			fprintf(stderr, "Failed to open %s: %s.\n",
					"/sys/bus/i2c/devices/1-001e/enable_device", strerror(errno));
		} 
	}
	write(mag_fd, &on_off, buf_size);
	(void)close(mag_fd);
}


// Handle signal interrupt
void sigHandler(int signo) {
	printf("SigINT received.\n");
	interrupt = 1;
}

static char *get_dev_path(void)
{
	static char path[128];
	int i, valid_str = 0;
	char buf[1024];
	FILE *fp;

	printf("Device detection, parsing " PROC_DEV "\n");
	
	if((fp = fopen(PROC_DEV, "r"))) {
		while(fgets(buf, sizeof buf, fp)) {
			switch(buf[0]) {
				case 'N':
					valid_str = strstr(buf, SENSOR_NAME) != 0;
					break;

				case 'H':
					if(valid_str) {
						char *ptr, *start;

						if(!(start = strchr(buf, '='))) {
							continue;
						}
						start++;

						if((ptr = strstr(start, "event"))) {
							start = ptr;
						}

						if((ptr = strchr(start, ' '))) {
							*ptr = 0;
						}
						if((ptr = strchr(start, '\n'))) {
							*ptr = 0;
						}

						snprintf(path, sizeof path, "/dev/input/%s", start);
						printf("Proc subsys says "SENSOR_NAME" sensor is mapped to %s \n", path);
						fclose(fp);
						return path;
					}
					break;

				case '\n':
					valid_str = 0;
					break;

				default:
					break;
			}
		}
	} 
	else {
		perror("Failed to open " PROC_DEV);
	}

	fprintf(stderr, "Trying alternative detection, querying /dev/input/eventX device names...\n");

	/* if for some reason we can't open the /proc/bus/input/devices file, or we
	 * couldn't find our device there, we'll try opening all /dev/input/eventX
	 * devices, and see if anyone is named: 3Dconnexion whatever
	 */
	i = 0;
	for(;;) {
		int fd;

		snprintf(path, sizeof path, "/dev/input/event%d", i++);

		fprintf(stderr, "  Trying \"%s\" ... ", path);

		if((fd = open(path, O_RDONLY)) == -1) {
			if(errno != ENOENT) {
				fprintf(stderr, "Failed to open %s: %s. This might hinder device detection\n",
						path, strerror(errno));
				continue;
			} 
			else {
				fprintf(stderr, "unknown\n");
				close(fd);
				break;
			}
		}

		if(ioctl(fd, EVIOCGNAME(sizeof buf), buf) == -1) {
			fprintf(stderr, "Failed to get device name for device %s: %s. This might hinder device detection\n",
					path, strerror(errno));
			buf[0] = 0;
		}

		fprintf(stderr, "%s\n", buf);

		if(strstr(buf, SENSOR_NAME)) {
			close(fd);
			printf("EVIOCGNAME says "SENSOR_NAME" sensor is mapped to %s \n", path);
			return path;
		}
	}
	return(0);  //should never get here
}

int main()
{
    int fd;
    struct input_event ie;
    float mag_x = OUT_OF_RANGE;
    float mag_y = OUT_OF_RANGE;
    float mag_z = OUT_OF_RANGE;
    float total_mf;
	static char *dev_path;
	int i;

	char on = '1';
	char off = '0';
	
	//catch interrupt signal
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	//Detect which /dev/input/event<n> file the sensor is mapped to
	dev_path=get_dev_path();
//	printf("Device path is %s \n", dev_path);
	
	//Open the /dev/input/event<n> file
    if((fd = open(dev_path, O_RDONLY)) == -1) {
        perror("Opening "SENSOR_NAME" device");
        exit(EXIT_FAILURE);
    }

	//Enable the sensor.  It is disabled by default at boot time.
	printf("Enabling the "SENSOR_NAME" sensor.\n");
	toggle_mag_sensor(on); //enable the magnetometer sensor

	//Do this until the user types <ctrl>-c
	while(!interrupt) {

		/*
		 * Batch read the sensor data
		 * There are 7 possible lines that can be read from the event file
		 * Read 7 lines at a time so we can be sure we always fetching fresh data
		 * and then pick the type 4 (sensor data)
		 * and sort for the x, y, and z axis magnetometer readings
		 */
		
 		for (i=0;i<7;i++) {
 			//Read the event file
 			if (read(fd, &ie, sizeof(struct input_event)) != 0) {
//				printf("type %d\tcode %d\tvalue %d\n", ie.type, ie.code, ie.value);
 				if (ie.type == 4){
 					switch (ie.code){
  	    				case 0:  //X axis
  	      					mag_x = (float)ie.value;
// 	      					printf("X axis code %d\t value %d\n", ie.code, ie.value);
  							break;
  	    				case 1:  //Y axis
  	      					mag_y = (float)ie.value;
// 	      					printf("Y axis code %d\t value %d\n", ie.code, ie.value);
  							break;
  	    				case 2:   //Temperature
  	      					mag_z = (float)ie.value;
// 	      					printf("Z axis code %d\t value %d\n", ie.code, ie.value);
  							break;
  	    				default:
  	      					break;
 	 	    		}
  	    		}
 			}
 	  		else {
 	    		printf("Sensor read failed...aborting\n");
 	    		break;
 	  		}
 		}
	   	if (mag_x != OUT_OF_RANGE && mag_y != OUT_OF_RANGE && mag_z != OUT_OF_RANGE) {
			printf("Magnetometer X axis = %4.0f ugauss, Y axis = %4.0f ugauss, Z axis = %4.0f ugauss \n", mag_x, mag_y, mag_z);
			printf("Magnetometer X axis = %4.0f uT, Y axis = %4.0f uT, Z axis = %4.0f uT \n", (mag_x/10000), (mag_y/10000), (mag_z/10000));
			total_mf = sqrt(pow(mag_x,2) + pow(mag_y,2) + pow(mag_z,2));
			printf("Total magnetic field = %4.0f ugauss or %4.1f nT.\n", (total_mf), (total_mf/10));
			printf("\n");
		}

		sleep(1);
	}

	printf("Disabling the "SENSOR_NAME" sensor.\n");
	toggle_mag_sensor(off); //disable the magnetometer sensor
	(void) close(fd);
    return 0;
}

