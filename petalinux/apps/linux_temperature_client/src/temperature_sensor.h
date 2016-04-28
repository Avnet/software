//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET
//       *======*
//        *====*
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
// Module Name:         temperature_sensor.h
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


#ifndef TEMPERATURE_SENSOR_H_
#define TEMPERATURE_SENSOR_H_

// System Includes.
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define DEVICE_NAME "/dev/spidev32766.0"

// MAX31723 Register address definitions.
#define MAX31723_TEMP_READ		(0x01)  // Temperature LSB Register (read only)
#define MAX31723_CSR_WRITE		(0x80)  // Config/Status Register (write only)

// MAX31723 Register bit definitions.
#define MAX31723_CSR_R1			(2) // Bit location for thermostat resolution bit 1.
#define MAX31723_CSR_R0			(1) // Bit location for thermostat resolution bit 0.
#define MAX31723_12BIT_MODE		((1 << MAX31723_CSR_R0) | (1 << MAX31723_CSR_R1))  // 12-bit resolution CSR setting.

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* Retrieve formatted temperature data from the MAX31723 temperature sensor
 * module which is connected via the Zynq Programmable Logic. */
int get_temperature(float* temperature_result, int verbosity);

#endif // TEMPERATURE_SENSOR_H_
