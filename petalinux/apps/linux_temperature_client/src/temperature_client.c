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
 *    Kevin Keryk  - Modified version of Quickstart to allow temperature data
 *                   to be published from Avnet Zed boards
 *******************************************************************************/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "iotfclient.h"
#include "temperature_sensor.h"

#define BOARD_NAME_STRING           "ZED"
#define RANDOM_ID_SIZE              3
#define USER_INPUT_BUFFER_SIZE		1024

volatile int interrupt = 0;
struct timeval timeout_value;

// Handle signal interrupt
void sigHandler(int signo) {
	printf("SigINT received.\n");
	interrupt = 1;
}

// Create specified number of random numeric ASCII digits within a buffer.
int getrandom(char * buf, size_t random_digit_count)
{
	int fd_random_data;
	int index = 0;
	int length = 0;

	// Open /dev/urandom where the random data will come from.
	fd_random_data = open("/dev/urandom", O_RDONLY);

	while (index < random_digit_count)
	{
		length = read(fd_random_data, buf + index, (random_digit_count - index));

		if (length < 0)
		{
			int errsv = errno;
			printf("Error (%s) reading %d bytes from /dev/urandom device %d pass (%d)\n", strerror(errsv), (random_digit_count - index), fd_random_data, index);
			return -1;
		}

		index += length;
	}

	// Finished reading data from /dev/urandom device.
	close(fd_random_data);

	if (index != random_digit_count)
	{
		printf("Error generating Random ID, only %d chars, not %d chars!\n", length, random_digit_count);
	}
	else
	{
		// Truncate the random bytes to base 10 and convert to ASCII digits.
		for (index = 0; index < random_digit_count; index++)
		{
			buf[index] = (buf[index] % 10) + 0x30;
		}

		// Terminate the string with a null character.
		buf[index] = '\0';
	}

	return index;
}

int main(int argc, char *argv[])
{
	char* randomId = malloc(sizeof(char) * (RANDOM_ID_SIZE + 1));
	char* deviceId = BOARD_NAME_STRING;
	char* randomDeviceId = malloc((sizeof(char) * (sizeof(deviceId) + RANDOM_ID_SIZE + 1)));
	char read_buffer[USER_INPUT_BUFFER_SIZE];
	char user_input_buffer[USER_INPUT_BUFFER_SIZE];
	char *data_message_format = "{\"d\" : {\"temperature\" : %f }}";
	char data_message_content[USER_INPUT_BUFFER_SIZE];
	static char* data_enable_command = "DataEnable";
	static char* data_disable_command = "DataDisable";
	int rc = -1;
	int fd_enable_stream;
	int fd_temperature_data;
	float data = 13;
	int input_result;
	int ret_val;

	fd_set fdlist;
	ssize_t numread = 0;
	ssize_t numwrite = 0;

	//catch interrupt signal
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	Iotfclient client;

	// Create a 3 digit random ID and add it to the device ID string.
	if (getrandom(randomId, RANDOM_ID_SIZE) != RANDOM_ID_SIZE)
	{
		// Create a device ID with non-random numbers.
		sprintf(randomDeviceId, "%s123", deviceId);
	}
	else
	{
		// Concatenate the device ID and random digit strings to form a
		// pseudo-unique device ID.
		strcpy(randomDeviceId, deviceId);
		strcat(randomDeviceId, randomId);
	}
	free(randomId);

	// Create the named pipe to request new data.
	ret_val = mkfifo("/tmp/data_enable", 0666);
	if ((ret_val == -1) && (errno != EEXIST)) {
		printf("Error creating the /tmp/data_enable named pipe\n");
	}

	// Create the named pipe for new incoming temperature data.
	ret_val = mkfifo("/tmp/temperature_data", 0666);
	if ((ret_val == -1) && (errno != EEXIST)) {
		printf("Error creating the /tmp/data_enable named pipe\n");
	}

	// Open the named pipe used to request new data.
	fd_enable_stream = open("/tmp/data_enable", O_RDWR);

	// Open the temperature data pipe for reading.
	fd_temperature_data = open("/tmp/temperature_data", O_RDWR);

	// Write the data enable command to the request data pipe.
	numwrite = write(fd_enable_stream, data_enable_command, strlen(data_enable_command));
	if (numwrite > 0)
	{
		printf("Wrote %d char command to the /tmp/data_enable pipe: %s\n", numwrite, data_enable_command);
	}

	// for Quickstart, no need to pass the authmethod and token(last 2 params)
	rc = initialize(&client,"quickstart","iotsample",randomDeviceId,NULL,NULL);

	// For registered mode
	// rc = initialize(&client,"ordid","deviceType","deviceid","token","r+i2I720I+EkZe1c@8");

	if(rc != SUCCESS){
		printf("Initialize returned rc = %d.\n Quitting..\n", rc);
		return 0;
	}

	rc = connectiotf(&client);

	if(rc != SUCCESS){
		printf("Connection returned rc = %d.\n Quitting..\n", rc);
		return 0;
	}

	printf("Connection Successful. Press Ctrl+C to quit\n");
	printf("View the visualization at https://quickstart.internetofthings.ibmcloud.com/#/device/%s\n", randomDeviceId);

	/* Format the initial data into the data message content to be
	 * sent to the cloud service. */
	sprintf(data_message_content, data_message_format, data);

	while(!interrupt) 
	{
		/* Establish file descriptor lists for stdin, fdCornerRequest, and
		 * fdThresholdValue reading. */
		FD_ZERO(&fdlist);
		FD_SET(fd_temperature_data, &fdlist);

		// Set the timeout period to match the desired select timeout period.
		timeout_value.tv_sec = 1;
		timeout_value.tv_usec = 0;

		// Check to see if user is entering input onto the console.
		switch (select(fd_temperature_data + 1, &fdlist, NULL, NULL, &timeout_value))
		{

		case 0:
			//printf("MESSAGE: Select() returned after timeout period elapsed.\n");
		break;

		case -1:
			printf("select() returned -1.");
			exit(1);
		break;

		default:
			/* Check to see if there is input coming from
			 * fd_temperature_data named pipe. */
			if (FD_ISSET(fd_temperature_data, &fdlist))
			{
				// Read a temperature value coming in from the temperature pipe.
				numread = read(fd_temperature_data, read_buffer, USER_INPUT_BUFFER_SIZE);

				if (numread > 0)
				{
					read_buffer[numread] = '\0';
					printf("Read %d from the /tmp/temperature_data pipe: %s\n", numread, read_buffer);
				}

				// Evaluate the entered value as a decimal value.
				sscanf((char*)read_buffer, "%d", (int*)&input_result);

				// Sanity check on the temperature data.
				if (input_result >= 0)
				{
					data = input_result;
					printf("MESSAGE:  Accepted updated temperature data of %f\n", data);
				}
				else
				{
					printf("MESSAGE:  Invalid threshold value entered, using old threshold value of %f\n", data);
				}

				// Clear the input buffer and temporary read variables.
				input_result = 0;
				read_buffer[0] = (char) 0;
			}
		break;
		} // END: switch (select(fd_temperature_data + 1, &fdlist, NULL, NULL, &timeout_value))

		// Check the user input buffer for some valid input.
		if (user_input_buffer[0] != (char) 0)
		{
			// Evaluate the entered value as a decimal value.
			sscanf((char*)user_input_buffer, "%d", (int*)&input_result);

			// Sanity check on the specified temperature value.
			if ((input_result >= 0) && (input_result <= 100))
			{
				data = (float) input_result;
				printf("MESSAGE:  Accepted updated temperature value of %f\n", data);
			}
			else
			{
				printf("MESSAGE:  Invalid temperature value entered, using old temperature value of %f\n", data);
			}

			// Clear the input buffer and temporary read variables.
			input_result = 0;
			user_input_buffer[0] = (char) 0;
		}
		else
		{
			// If no user input was sent, try to capture data from the
			// MAX31723 temperature sensor.
			ret_val = get_temperature(&data, 0);

			if (ret_val != 0)
			{
				printf("MESSAGE:  Unable to capture data from temperature sensor\n");
				data = 11.0f;
			}
		}

		 /* Format the updated data into the data message contents to be
		  * sent to the cloud service. */
		sprintf(data_message_content, data_message_format, data);

		//printf("Publishing the event stat with rc ");
		rc= publishEvent(&client,"status","json", (unsigned char*) data_message_content, QOS0);
	}

	printf("Quitting!!\n");

	// Disconnect the MQTT Client.
	disconnect(&client);

	// Write the data disable command to the request data pipe.
	numwrite = write(fd_enable_stream, data_disable_command, strlen(data_disable_command));
	if (numwrite > 0)
	{
		printf("Wrote %d char command to the /tmp/data_enable pipe: %s\n", numwrite, data_disable_command);
	}

	// Close the open file descriptors.
	close(fd_enable_stream);
	close(fd_temperature_data);

	// Free up allocated memory space.
	free(randomDeviceId);

	return 0;
}

