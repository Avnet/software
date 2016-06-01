/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */
 
/*
 * Modified for the Maxim 31855 SPI Sensor
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
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "PmodUtilities.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

//static const char *device = "/dev/spidev32766.0";   // Default for PetaLinux
static const char *device = "/dev/spidev0.0";   	// Default for Pulsar 7
static uint8_t mode = SPI_CPHA;						// Xilinx PL SPI IP requires a clock phase shift to align the data
static uint8_t bits = 8;
static uint32_t speed = 3125000;					// Maximum speed for the Xilinx PL SPI IP, as confgured in Vivado
static uint16_t delay;

int readMaxim31855(int fd)
{
	int ret;
	int nTemporaryValue=0;
	int nTemporaryValue2=0;
	float MAX31855_internal_temp=0.0f;
	float MAX31855_thermocouple_temp=0.0f;

	uint8_t tx_read[] = {
		0x0, 0x0, 0x0, 0x0, 0x0,
	};
	
	uint8_t rx[ARRAY_SIZE(tx_read)] = {0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_read,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx_read),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	for (ret=0; ret < ARRAY_SIZE(tx_read); ret++)
		rx[ret] = 0xFF;

	tr.len = ARRAY_SIZE(tx_read);
	tr.tx_buf = (unsigned long)tx_read;
	
	//	printf("Sending: 0x%X 0x%X 0x%X\n", tx_read[0], tx_read[1], tx_read[2]);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("Can't read temperature data from MAXIM 31855");
	
	// Check for error codes from the Maxim 31855 Sensor
	if (rx[3] & 0x01) 
		pabort("ERROR from 31855: Open Circuit");
	else if (rx[3] & 0x02)
		pabort("ERROR from 31855: Short to Ground");
	else if (rx[3] & 0x04)
		pabort("ERROR from 31855: Short to VCC");
	else if (rx[1] & 0x01)
		pabort("ERROR from 31855: Unspecified Fault");
	
	//	printf("Receiving: 0x%X 0x%X 0x%X\n", rx[0], rx[1], rx[2]);
	// Retrieve Internal Sensor Temperature 
	nTemporaryValue = rx[2];  			// bits 11..4
	nTemporaryValue = nTemporaryValue << 4;		// shift left to make room for bits 3..0
	nTemporaryValue2 = rx[3];					// bits 3..0 in the most significant spots
	nTemporaryValue2 = nTemporaryValue2 >> 4;	// shift right to get rid of extra bits and position
	nTemporaryValue |= nTemporaryValue2;		// Combine to get bits 11..0
	
	if((rx[2] & 0x80)==0x80)					// Check the sign bit and sign-extend if need be
		nTemporaryValue |= 0xFFFFF800;
		
	MAX31855_internal_temp = (float)nTemporaryValue / 16.0f;

	// Thermocouple Temp
	nTemporaryValue = rx[0];  					// bits 13..6
	nTemporaryValue = nTemporaryValue << 6;		// shift left to make room for bits 5..0
	nTemporaryValue2 = rx[1];				// bits 5..0 in the most significant spots
	nTemporaryValue2 = nTemporaryValue2 >> 2;	// shift right to get rid of extra bits and position
	nTemporaryValue |= nTemporaryValue2;		// Combine to get bits 13..0
	
	if((rx[0] & 0x80)==0x80)				// Check the sign bit and sign-extend if need be
		nTemporaryValue |= 0xFFFFE000;
		
	MAX31855_thermocouple_temp = (float)nTemporaryValue / 4.0f;

	printf("MAX31855 Internal Temp     = %.1f deg C or %.2f deg F\n\r", MAX31855_internal_temp, MAX31855_internal_temp*1.8 + 32);
	printf("MAX31855 Thermocouple Temp = %.1f deg C or %.2f deg F\n\r", MAX31855_thermocouple_temp, MAX31855_thermocouple_temp*1.8 + 32);

	return((int)(MAX31855_internal_temp * 100));
}


int SPI_Init()
{
	int ret = 0;
	int fd;

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	
	return (fd);
	
}

int SPI_Close(int fd)
{
	return (close(fd));
}
