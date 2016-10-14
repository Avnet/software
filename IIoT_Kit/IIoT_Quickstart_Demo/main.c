/*******************************************************************************
 * Copyright (c) 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Jeffrey Dare - initial implementation and API implementation
 * 
 * Modified for the sampleDevice program from the OpenSource Paho Examples
 *
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
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <linux/input.h>

#include "iotfclient.h"
#include "PmodUtilities.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define PROC_DEV		"/proc/bus/input/devices"
#define SENSOR_NAME 	"hts221"

volatile int interrupt = 0;

int readHTS221(int fd, int type);

char *sensorStr[] = {"HTS221 Temperature", 
					 "HTS221 Humidity", 
					 "MAX31855 Internal Temperature",
					 "MAX31855 Thermocouple Temperature",
					 "No Sensor - generated data"};
					 
enum mode { i2ct, i2ch, spii, spit, mock } dataSource = i2ct;

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

void help()
{
	int idx;
	printf("\n\nDefault no argument - read from %s\n", sensorStr[i2ct]);
	
	for (idx = i2ct; idx <= mock; idx++) {
		printf("                  %d - read from %s\n", idx, sensorStr[idx]);
	}
	
	exit(0);
}

char *pack_json(char *json_str, int value)
{
        char * ptr;

	ptr = json_str + 16;
	(void) sprintf(ptr, "%7.2f", (float)(value/100.0));
	(void) strcat(json_str, " }}");

	printf("JSON = %s\n", json_str);

	return json_str;
}

int main(int argc, char const *argv[])
{
	int rc = -1;
	int gpio_value = 0;
	int pb_value = 0;
	int nchannel = 0;
	char json_str[32], *ptr;
	int sensorVal = 1800;
	int increment = 50;
	int fd = 0;
	static char *dev_path;

	Iotfclient client;
	char * deviceId = NULL;
	
	//catch interrupt signal
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	dev_path=get_dev_path();
	printf("Device path is %s \n", dev_path);
	
	if (argc == 1) {
		printf("You must specify a unique ID...exiting.\n");
		return 0;
	}

    deviceId = (char *)argv[1];
    
	if (argc > 2) {
		if (*argv[2] == '-') {
			help();
		}
		else {
			dataSource = (*argv[2] - '0') % ARRAY_SIZE(sensorStr);
		}
	}

	// for Quickstart, no need to pass the authmethod and token(last 2 params)
	rc = initialize(&client,"quickstart","iotsample",deviceId,NULL,NULL);

	if(rc != SUCCESS){
		printf("initialize failed and returned rc = %d.\n Quitting..", rc);
		return 0;
	}

	rc = connectiotf(&client);

	if(rc != SUCCESS){
		printf("Connection failed and returned rc = %d.\n Quitting..", rc);
		return 0;
	}

	printf("Connection Successful. Press Ctrl+C to quit\n");
	printf("View the visualization at https://quickstart.internetofthings.ibmcloud.com/#/device/%s\n", deviceId);

	if (dataSource == spii || dataSource == spit) {
		fd = SPI_Init();
		
		if (fd == 0) {
			printf("Cannot open SPI device\n");
			return -1;
		}
	}
	else if (dataSource < spii) {
        if((fd = open(dev_path, O_RDONLY)) == -1) {
                perror("opening HTS221 device");
                exit(EXIT_FAILURE);
        }
	}

	while(!interrupt) 
	{
		switch(dataSource){
			case i2ct:
				sensorVal = readHTS221(fd, i2ct);
			  	break;
			case i2ch:
				sensorVal = readHTS221(fd, i2ch);
			  	break;
			case spii:
				sensorVal = readMaxim31855(fd, spii);
			  	break;
			case spit:
				sensorVal = readMaxim31855(fd, spit);
			  	break;
			case mock:
				increment = (sensorVal > 2800 || sensorVal < 1800) ? -increment : increment;
				sensorVal += increment; 
			    break;
			default:  // i2ct
				sensorVal = readHTS221(fd,i2ct);
			  	break;
		}
		
		//		printf("Integer Temp Value = %d\n", sensorVal);
		printf ("-----Sensor reading from %s\n", sensorStr[dataSource]);
		
		if (dataSource == i2ch)
			ptr = strcpy(json_str, "{\"d\" : {\"humd\" : XXXXXXX }}");
		else
			ptr = strcpy(json_str, "{\"d\" : {\"temp\" : XXXXXXX }}");

		ptr = pack_json(ptr, sensorVal);
		printf("Publishing the event stat with rc ");
		rc= publishEvent(&client, "status","json", json_str, QOS0);
		printf(" %d. Type <CTRL>-C to terminate...\n", rc);
		yield(&client, 3000);
	}

	printf("Quitting!!\n");
	
	if (dataSource == spii || dataSource == spit)
		SPI_Close(fd);
	else if (dataSource == i2ct || dataSource == i2ch)
		(void) close(fd);

	disconnect(&client);

	return 0;
}
