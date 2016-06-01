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

#ifndef GPIO_DEVICES_H_
#define GPIO_DEVICES_H_

#define GPIO_LED 47
#define GPIO_PB  51

// #define GPIO_BASE 906                   // PetaLinux 2015.2.1
#define GPIO_BASE 0                     // Pulsar 7
#define GPIO_ROOT "/sys/class/gpio"
#define POLL_DELAY_USECS (500000)

int open_gpio_channel(int gpio_base);
int close_gpio_channel(int gpio_base);
int set_gpio_direction(int gpio_base, int nchannel, char *direction);
int set_gpio_value(int gpio_base, int nchannel, int value);
int get_gpio_value(int gpio_base, int nchannel);


#endif /* GPIO_DEVICES_H_ */
