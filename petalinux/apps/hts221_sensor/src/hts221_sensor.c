
/*
 * Modified for the ST Microelectronics HTS221 Sensor
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

#include <linux/input.h>

#define OUT_OF_RANGE -500.0
#define PROC_DEV		"/proc/bus/input/devices"
#define SENSOR_NAME 	"hts221"

volatile int interrupt = 0;

void toggle_temp_sensor(char on_off) {
	int temp_fd;
	int buf_size = 1;
	
	if ((temp_fd = open("/sys/bus/i2c/devices/1-005f/enable_device", O_WRONLY)) == -1) {
		if(errno != ENOENT) {
			fprintf(stderr, "Failed to open %s: %s.\n",
					"/sys/bus/i2c/devices/1-005f/enable_device", strerror(errno));
		} 
	}
	write(temp_fd, &on_off, buf_size);
	(void)close(temp_fd);
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
	float humidity = OUT_OF_RANGE;
	float temperature = OUT_OF_RANGE;
	static char *dev_path;

	char on = '1';
	char off = '0';

	//catch interrupt signal
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	dev_path=get_dev_path();
//	printf("Device path is %s \n", dev_path);
	
    if((fd = open(dev_path, O_RDONLY)) == -1) {
		perror("opening "SENSOR_NAME" device");
		exit(EXIT_FAILURE);
	}

	//Enable the sensor.  It is disabled by default at boot time.
	printf("Enabling the "SENSOR_NAME" sensor.\n");
	toggle_temp_sensor(on); //enable the temperature sensor

	while(!interrupt) {
		if (read(fd, &ie, sizeof(struct input_event)) != 0) {
//			printf("type %d\tcode %d\tvalue %d\n", ie.type, ie.code, ie.value);
			switch (ie.code){
				case 40:  //Relative Humidity
					humidity = (float)ie.value/1000.0;
			 		break;
			 	case 9:   //Temperature
			 		temperature = (float)ie.value/1000.0;
			 		break;
			 	default:
			 		break;
			}
		}
		else {
			printf("Sensor read failed...aborting\n");
			break;
		}
		if (humidity != OUT_OF_RANGE && temperature != OUT_OF_RANGE) {
		 	printf("Humidity = %5.2f%%, Temperature = %5.2f C or %5.2f F\n", humidity, temperature, temperature * 1.8 + 32);
		 	humidity = temperature = OUT_OF_RANGE;
		}
		sleep(1);
	}
	
	printf("Disabling the "SENSOR_NAME" sensor.\n");
	toggle_temp_sensor(off); //disable the temperature sensor
	(void) close(fd);
	return 0;
}

