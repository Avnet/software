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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "iotfclient.h"
#include "gpio-devices.h"
#include "PmodUtilities.h"

#define EVENT_FILE 	"/dev/input/event0"
#define CONFIG_PATH "/root/device.cfg"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

volatile int interrupt = 0;
static int gl_gpio_base = GPIO_BASE;

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

void help()
{
	int idx;
	printf("\n\nDefault no argument - read from %s\n", sensorStr[i2ct]);
	
	for (idx = i2ct; idx <= mock; idx++) {
		printf("                  %d - read from %s\n", idx, sensorStr[idx]);
	}
	
	exit(0);
}

void myCallback (char* commandName, char* format, void* payload)
{
	printf("------------------------------------\n" );
	printf("The command received :: %s\n", commandName);
	printf("format : %s\n", format);
	printf("Payload is : %s\n", (char *)payload);

	printf("------------------------------------\n" );
	
	if (strchr((char *) payload, 'c') != NULL)
		set_gpio_value(gl_gpio_base, GPIO_LED, 1);
	else 
		set_gpio_value(gl_gpio_base, GPIO_LED, 0);
}

char *pack_json(char *json_str, int value)
{
        char * ptr;

	ptr = json_str + 16;
	(void) sprintf(ptr, "%5d", value);
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
	
	Iotfclient client;
	
	//catch interrupt signal
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	char *configFilePath = CONFIG_PATH;
	
	if (argc > 1) 
		if (*argv[1] == '-')
			help();
		else
			dataSource = (*argv[1] - '0') % ARRAY_SIZE(sensorStr);

//	dataSource = spii;
	
	rc = initialize_configfile(&client, configFilePath);

	if(rc != SUCCESS){
		printf("initialize failed and returned rc = %d.\n Quitting..", rc);
		return 0;
	}

	rc = connectiotf(&client);

	if(rc != SUCCESS){
		printf("Connection failed and returned rc = %d.\n Quitting..", rc);
		return 0;
	}

	setCommandHandler(&client, myCallback);
	
	nchannel = open_gpio_channel(gl_gpio_base);
	set_gpio_direction(gl_gpio_base, GPIO_LED, "out");
	set_gpio_direction(gl_gpio_base, GPIO_PB, "in");
	set_gpio_value(gl_gpio_base, GPIO_LED, gpio_value);
	pb_value=get_gpio_value(gl_gpio_base, GPIO_PB);
	//	fprintf(stdout,"Push button value = 0x%08X\n", pb_value);
	
	if (dataSource == spii || dataSource == spit) {
		fd = SPI_Init();
		
		if (fd == 0) {
			printf("Cannot open SPI device\n");
			return -1;
		}
	}
	else if (dataSource < spii) {
        if((fd = open(EVENT_FILE, O_RDONLY)) == -1) {
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
				if (get_gpio_value(gl_gpio_base,GPIO_PB) == 1) {  //pushbutton pressed
					sensorVal = 1800;
				}
				else {
					increment = (sensorVal > 2800 || sensorVal < 1800) ? -increment : increment;
					sensorVal += increment; 
				}
			    break;
			default:  // i2ct
				sensorVal = readHTS221(fd,i2ct);
			  	break;
		}
		
		//		printf("Integer Temp Value = %d\n", sensorVal);
		printf ("-----Sensor reading from %s\n", sensorStr[dataSource]);
		
		if (dataSource == i2ch)
			ptr = strcpy(json_str, "{\"d\" : {\"humd\" : XXXXX }}");
		else
			ptr = strcpy(json_str, "{\"d\" : {\"temp\" : XXXXX }}");
			
		ptr = pack_json(ptr, sensorVal);
		printf("Publishing the event stat with rc ");
		rc= publishEvent(&client, "status","json", json_str, QOS0);
		printf(" %d\n", rc);
		yield(&client, 3000);
	}

	printf("Quitting!!\n");
	
	if (dataSource == spii || dataSource == spit)
		SPI_Close(fd);
	else if (dataSource == i2ct || dataSource == i2ch)
		(void) close(fd);

	close_gpio_channel(gl_gpio_base);

	disconnect(&client);

	return 0;
}
