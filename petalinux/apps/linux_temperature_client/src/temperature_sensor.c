//----------------------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Please direct any questions to the PicoZed community support forum:
//    http://www.picozed.org/forum
//
// Product information is available at:
//    http://www.picozed.org/product/picozed
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2016 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Mar 30, 2016
// Design Name:         MicroZed Sensor Fusion IoT Platform
// Module Name:         temperature_sensor.c
// Project Name:        MicroZed Sensor Fusion IoT Platform
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     MicroZed, MicroZed IOCC Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         MicroZed Sensor Fusion User Space Driver
//
// Dependencies:
//
// Revision:            Mar 30, 2016: 1.00 Initial version
//
//----------------------------------------------------------------------------

// Local Includes.
#include "temperature_sensor.h"

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static uint8_t mode;
static uint8_t bits = 8;
static uint16_t delay = 0;
static uint32_t speed = 5000000;
static int verbose = 0;

static int initialize_temperature_sensor(char *device)
{
	int ret;
	int temperature_sensor_fd;
	uint8_t tx[] = {
		MAX31723_CSR_WRITE, MAX31723_12BIT_MODE,
	};
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	/* Open the SPI device which will be used to retrieve temperature data. */
	temperature_sensor_fd = open(device, O_RDWR);
	if (temperature_sensor_fd < 0)
		pabort("can't open device");

	/* Set the SPI mode parameters here.
	 */
	// Set SPI clock phase mode to 1 by default.
	mode |= SPI_CPHA;

	/*
	 * Assign the SPI mode to the device.
	 */
	ret = ioctl(temperature_sensor_fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");
	// Verify device SPI mode.
	ret = ioctl(temperature_sensor_fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	if (verbose)
	{
		printf("spi mode: %d\n", mode);
	}

	/* Send the slave device configuration message. */
	ret = ioctl(temperature_sensor_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

	if (verbose)
	{
		for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
			if (!(ret % 6))
				puts("");
			printf("%.2X ", rx[ret]);
		}
		puts("");
	}

	return temperature_sensor_fd;
}

static int read_temperature_sensor(int temperature_sensor_fd, uint8_t* temperature_lsb, uint8_t* temperature_msb)
{
	int ret;
	uint8_t tx[] = {
		MAX31723_TEMP_READ, 0x00, 0x00,
	};
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	/* Transmit message to read temperature data back from slave device. */
	ret = ioctl(temperature_sensor_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
	{
		printf("ERROR: read_temperature_sensor() can't send spi message\n");
		return -1;
	}

	if (verbose)
	{
		for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
			if (!(ret % 6))
				puts("");
			printf("%.2X ", rx[ret]);
		}
		puts("");
	}

	/* Extract the temperature data from the transfer buffer. */
	*temperature_lsb = rx[1];
	*temperature_msb = rx[2];

	return 0;
}

float convert_temperature_result(uint8_t* temperature_lsb, uint8_t* temperature_msb)
{
	int temperature_convert;
	float temperature_result;

	/* Convert temperature reading to floating point representation. */
	temperature_convert = (int)*temperature_msb;
	temperature_convert = temperature_convert << 4;
	temperature_convert += (int) ((*temperature_lsb & 0xF0) >> 4);

	/* If the 12th bit of the data is set, the other bits must be set for
	 * the 2s compliment negative value. */
	if ((temperature_convert & 0x00000800) == 0x00000800)
	{
		temperature_convert |= 0xFFFFF000;
	}

	/* Cast to floating point. */
	temperature_result = (float)temperature_convert;

	/* Each bit in temperature_result represents 1/16 of a degree. */
	temperature_result = (temperature_result / 16.0f);

	if (verbose)
	{
		printf("%.1f C\n", temperature_result);
	}

	return temperature_result;
}

int get_temperature(float* temperature_result, int verbosity)
{
	uint8_t temperature_lsb;
	uint8_t temperature_msb;
	int ret = 0;
	int fd = 0;

	/* Determine the level of verboseness. */
	if (verbosity != 1)
	{
		verbose = 0;
	}
	else
	{
		verbose = verbosity;
	}

	/* Initialize the temperature sensor to start data conversion. */
	fd = initialize_temperature_sensor(DEVICE_NAME);

	/* Read temperature data from the sensor. */
	ret = read_temperature_sensor(fd, &temperature_lsb, &temperature_msb);

	/* Convert the raw temperature data into a more meaningful floating
	 * point representation. */
	*temperature_result = convert_temperature_result(&temperature_lsb, &temperature_msb);

	/* Finished, close up file descriptors. */
	close(fd);

	return ret;
}

