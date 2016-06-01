/*
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
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "gpio-devices.h"

static int gl_gpio_base = GPIO_BASE;

int open_gpio_channel(int gpio_base)
{
	char gpio_nchan_file[128];
	int gpio_nchan_fd;
	int gpio_max;
	int nchannel;
	char nchannel_str[5];
	char *cptr;
	int c;
	char channel_str[5];

	char *gpio_export_file = "/sys/class/gpio/export";
	int export_fd=0;

	/* Check how many channels the GPIO chip has */
	sprintf(gpio_nchan_file, "%s/gpiochip%d/ngpio", GPIO_ROOT, gpio_base);
	gpio_nchan_fd = open(gpio_nchan_file, O_RDONLY);
	if (gpio_nchan_fd < 0) {
		fprintf(stderr, "Failed to open %s: %s\n", gpio_nchan_file, strerror(errno)); 
		return -1;
	}
	read(gpio_nchan_fd, nchannel_str, sizeof(nchannel_str));
	close(gpio_nchan_fd);
	nchannel=(int)strtoul(nchannel_str, &cptr, 0);
	if (cptr == nchannel_str) {
		fprintf(stderr, "Failed to change %s into GPIO channel number\n", nchannel_str);
		exit(1);
	}

	/* Open files for each GPIO channel */
	export_fd=open(gpio_export_file, O_WRONLY);
	if (export_fd < 0) {
		fprintf(stderr, "Cannot open GPIO to export %d\n", gpio_base);
		return -1;
	}

	gpio_max = gpio_base + nchannel;	
	for(c = gpio_base; c < gpio_max; c++) {
		sprintf(channel_str, "%d", c);
		write(export_fd, channel_str, (strlen(channel_str)+1));
	}
	close(export_fd);
	return nchannel;
}

int close_gpio_channel(int gpio_base)
{
	char gpio_nchan_file[128];
	int gpio_nchan_fd;
	int gpio_max;
	int nchannel;
	char nchannel_str[5];
	char *cptr;
	int c;
	char channel_str[5];

	char *gpio_unexport_file = "/sys/class/gpio/unexport";
	int unexport_fd=0;

	/* Check how many channels the GPIO chip has */
	sprintf(gpio_nchan_file, "%s/gpiochip%d/ngpio", GPIO_ROOT, gpio_base);
	gpio_nchan_fd = open(gpio_nchan_file, O_RDONLY);
	if (gpio_nchan_fd < 0) {
		fprintf(stderr, "Failed to open %s: %s\n", gpio_nchan_file, strerror(errno)); 
		return -1;
	}
	read(gpio_nchan_fd, nchannel_str, sizeof(nchannel_str));
	close(gpio_nchan_fd);
	nchannel=(int)strtoul(nchannel_str, &cptr, 0);
	if (cptr == nchannel_str) {
		fprintf(stderr, "Failed to change %s into GPIO channel number\n", nchannel_str);
		exit(1);
	}

	/* Close opened files for each GPIO channel */
	unexport_fd=open(gpio_unexport_file, O_WRONLY);
	if (unexport_fd < 0) {
		fprintf(stderr, "Cannot close GPIO by writing unexport %d\n", gpio_base);
		return -1;
	}

	gpio_max = gpio_base + nchannel;	
	for(c = gpio_base; c < gpio_max; c++) {
		sprintf(channel_str, "%d", c);
		write(unexport_fd, channel_str, (strlen(channel_str)+1));
	}
	close(unexport_fd);
	return 0;
}

int set_gpio_direction(int gpio_base, int nchannel, char *direction)
{
	char gpio_dir_file[128];
	int direction_fd=0;

	sprintf(gpio_dir_file, "/sys/class/gpio/gpio%d/direction",gpio_base + nchannel);
	direction_fd=open(gpio_dir_file, O_RDWR);
	if (direction_fd < 0) {
		fprintf(stderr, "Cannot open the direction file for GPIO %d\n", gpio_base + nchannel);
		return -1;
	}
	write(direction_fd, direction, (strlen(direction)+1));
	close(direction_fd);
	return 0;
}

int set_gpio_value(int gpio_base, int nchannel, int value)
{
	char gpio_val_file[128];
	int val_fd=0;
	char val_str[2];
	int c;

	sprintf(gpio_val_file, "/sys/class/gpio/gpio%d/value",gpio_base + nchannel);
	val_fd=open(gpio_val_file, O_RDWR);
	if (val_fd < 0) {
		fprintf(stderr, "Cannot open the value file of GPIO %d\n", gpio_base + nchannel);
		return -1;
	}
	sprintf(val_str,"%d", (value & 1));
	write(val_fd, val_str, sizeof(val_str));
	close(val_fd);
	return 0;
}

int get_gpio_value(int gpio_base, int nchannel)
{
	char gpio_val_file[128];
	int val_fd=0;
	char val_str[2];
	char *cptr;
	int value = 0;

	sprintf(gpio_val_file, "/sys/class/gpio/gpio%d/value",gpio_base + nchannel);
	val_fd=open(gpio_val_file, O_RDWR);
	if (val_fd < 0) {
		fprintf(stderr, "Cannot open GPIO to export %d\n", gpio_base + nchannel);
		return -1;
	}
	read(val_fd, val_str, sizeof(val_str));
	value += (int)strtoul(val_str, &cptr, 0);
	if (cptr == optarg) {
		fprintf(stderr, "Failed to change %s into integer", val_str);
	}
	close(val_fd);
	return value;
}
