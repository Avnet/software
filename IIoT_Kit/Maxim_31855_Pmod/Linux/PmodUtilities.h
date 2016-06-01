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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#ifndef __PMODUTILITES_H_
#define __PMODUTILITES_H_
#include <stdio.h>

#define SPI_CHANNEL_SEL_0		0xFFFFFFFE					// Select spi channel 0
#define SPI_CHANNEL_SEL_1		0xFFFFFFFD					// Select spi channel 1
#define SPI_CHANNEL_SEL_NONE	0xFFFFFFFF					// Deselect all SPI channels

// Initialization settings for the AXI SPI controller's Control Register when addressing the MAX31855
// 0x186 = b1_1000_0110
//			1	Inhibited to hold off transactions starting
//			1	Manually select the slave
//			0	Do not reset the receive FIFO at this time
//			0	Do not reset the transmit FIFO at this time
//			0	Clock phase of 0
//			0	Clock polarity of low
//			1	Enable master mode
//			1	Enable the SPI Controller
//			0	Do not put in loopback mode

#define AXI_SPI_RESET_VALUE			0x0A  //!< Reset value for the AXI SPI Controller

#define MAX31855_CLOCK_PHASE_CPHA		0
#define MAX31855_CLOCK_POLARITY_CPOL	0
#define MAX31855_CR_INIT_MODE		XSP_CR_TRANS_INHIBIT_MASK | XSP_CR_MANUAL_SS_MASK   | \
		 						    XSP_CR_MASTER_MODE_MASK   | XSP_CR_ENABLE_MASK
#define MAX31855_CR_UNINHIBIT_MODE	                            XSP_CR_MANUAL_SS_MASK   | \
									XSP_CR_MASTER_MODE_MASK   | XSP_CR_ENABLE_MASK
#define AXI_SPI_RESET_VALUE			0x0A  //!< Reset value for the AXI SPI Controller

/************************** Function Prototypes *******************************/
int readMaxim31855(int fd);
int SPI_Init();
int SPI_Close(int fd);

#endif
