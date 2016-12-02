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
//  This design is the property of Avnet.  Publication of this
//  design is not authorized without written consent from Avnet.
// 
//  Please direct any questions to the UltraZed community support forum:
//     http://www.ultrazed.org/forum
// 
//  Product information is available at:
//     http://www.ultrazed.org/product/ultrazed-EG
// 
//  Disclaimer:
//     Avnet, Inc. makes no warranty for the use of this code or design.
//     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//     any errors, which may appear in this code, nor does it make a commitment
//     to update the information contained herein. Avnet, Inc specifically
//     disclaims any implied warranties of fitness for a particular purpose.
//                      Copyright(c) 2016 Avnet, Inc.
//                              All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Nov 18, 2016
// Design Name:         PS Pmod loopback test for UltraZed I/O Carrier
// Module Name:         linux_ps_pmod_loopback_test.c
// Project Name:        PS Pmod loopback test for UltraZed I/O Carrier
// Target Devices:      Xilinx Zynq UltraScale+ MPSoC
// Hardware Boards:     UltraZed-EG SOM and UltraZed I/O Carrier
//
// Tool versions:       Xilinx Vivado 2016.2
//                      Petalinux 2016.2
//
// Description:         PS Pmod loopback test application for Linux.
//
// Dependencies:
//
// Revision:            Nov 18, 2016: 1.00 Initial version
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*****************************************************************************
* 
* Here is the loopback map on UltraZed-EG SOM + IO Carrier Card PS Pmod:
*
*     PMOD_D0 (MIO41) <---> PMOD_D4 (MIO36)
*     PMOD_D1 (MIO43) <---> PMOD_D5 (MIO37)
*     PMOD_D2 (MIO42) <---> PMOD_D6 (MIO39)
*     PMOD_D3 (MIO38) <---> PMOD_D7 (MIO40)
*
*
*****************************************************************************/

#define PMOD_D0_GPIO	     379 // MIO41 = GPIO379
#define PMOD_D1_GPIO	     381 // MIO43 = GPIO381
#define PMOD_D2_GPIO	     380 // MIO42 = GPIO380
#define PMOD_D3_GPIO	     376 // MIO38 = GPIO376
#define PMOD_D4_GPIO	     374 // MIO36 = GPIO374
#define PMOD_D5_GPIO	     375 // MIO37 = GPIO375
#define PMOD_D6_GPIO	     377 // MIO39 = GPIO377
#define PMOD_D7_GPIO	     378 // MIO40 = GPIO378

int first_walking_ones_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// First ones step on IO.
	gpio379_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -11;
		printf("Walking ones test failure on GPIO379 to GPIO374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -12;
		printf("Walking ones test failure on GPIO381 to GPIO375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -13;
		printf("Walking ones test failure on GPIO380 to GPIO377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -14;
		printf("Walking ones test failure on GPIO376 to GPIO378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int second_walking_ones_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// Second ones step on IO.
	gpio379_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -21;
		printf("Walking ones test failure on GPIO379 to GPIO374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -22;
		printf("Walking ones test failure on GPIO381 to GPIO375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -23;
		printf("Walking ones test failure on GPIO380 to GPIO377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -24;
		printf("Walking ones test failure on GPIO376 to GPIO378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int third_walking_ones_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// Third ones step on third IO pair.
	gpio379_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -31;
		printf("Walking ones test failure on GPIO379 to GPIO374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -32;
		printf("Walking ones test failure on GPIO381 to GPIO375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -33;
		printf("Walking ones test failure on GPIO380 to GPIO377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -34;
		printf("Walking ones test failure on GPIO376 to GPIO378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int fourth_walking_ones_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// Fourth ones step on IO pair.
	gpio379_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -41;
		printf("Walking ones test failure on GPIO379 to GPIO374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -42;
		printf("Walking ones test failure on GPIO381 to GPIO375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -43;
		printf("Walking ones test failure on GPIO380 to GPIO377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -44;
		printf("Walking ones test failure on GPIO376 to GPIO378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}


int first_walking_zeros_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// First ones step on IO.
	gpio379_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -51;
		printf("Walking zeros test failure on gpio379 to gpio374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -52;
		printf("Walking zeros test failure on gpio381 to gpio375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -53;
		printf("Walking zeros test failure on gpio380 to gpio377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -54;
		printf("Walking zeros test failure on gpio376 to gpio378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int second_walking_zeros_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// Second ones step on IO.
	gpio379_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -61;
		printf("Walking zeros test failure on gpio379 to gpio374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -62;
		printf("Walking zeros test failure on gpio381 to gpio375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -63;
		printf("Walking zeros test failure on gpio380 to gpio377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -64;
		printf("Walking zeros test failure on gpio376 to gpio378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int third_walking_zeros_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// Third ones step on third IO pair.
	gpio379_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -71;
		printf("Walking zeros test failure on gpio379 to gpio374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -72;
		printf("Walking zeros test failure on gpio381 to gpio375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -73;
		printf("Walking zeros test failure on gpio380 to gpio377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -74;
		printf("Walking zeros test failure on gpio376 to gpio378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int fourth_walking_zeros_step(void)
{
	char gpio_setting[5];
	int gpio374_value;
	int gpio375_value;
	int gpio381_value;
	int gpio380_value;
	int gpio376_value;
	int gpio379_value;
	int gpio377_value;
	int gpio378_value;
	int test_result = 0;

	FILE  *fp_pmod_d0;
	FILE  *fp_pmod_d1;
	FILE  *fp_pmod_d2;
	FILE  *fp_pmod_d3;
	FILE  *fp_pmod_d4;
	FILE  *fp_pmod_d5;
	FILE  *fp_pmod_d6;
	FILE  *fp_pmod_d7;

	// Open the gpio value properties so that they can be read/written.
	fp_pmod_d0 = fopen("/sys/class/gpio/gpio379/value", "r+");
	fp_pmod_d1 = fopen("/sys/class/gpio/gpio381/value", "r+");
	fp_pmod_d2 = fopen("/sys/class/gpio/gpio380/value", "r+");
	fp_pmod_d3 = fopen("/sys/class/gpio/gpio376/value", "r+");
	fp_pmod_d4 = fopen("/sys/class/gpio/gpio374/value", "r+");
	fp_pmod_d5 = fopen("/sys/class/gpio/gpio375/value", "r+");
	fp_pmod_d6 = fopen("/sys/class/gpio/gpio377/value", "r+");
	fp_pmod_d7 = fopen("/sys/class/gpio/gpio378/value", "r+");

	// Fourth ones step on IO pair.
	gpio379_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d0);
	fflush(fp_pmod_d0);

	gpio381_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d1);
	fflush(fp_pmod_d1);

	gpio380_value = 1;
	strcpy(gpio_setting, "1");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d2);
	fflush(fp_pmod_d2);

	gpio376_value = 0;
	strcpy(gpio_setting, "0");
	fwrite(&gpio_setting, sizeof(char), 1, fp_pmod_d3);
	fflush(fp_pmod_d3);

	// Read the looped back PMOD_D4 values.
	fscanf(fp_pmod_d4, "%d", &gpio374_value);
	fscanf(fp_pmod_d5, "%d", &gpio375_value);
	fscanf(fp_pmod_d6, "%d", &gpio377_value);
	fscanf(fp_pmod_d7, "%d", &gpio378_value);

	if (gpio379_value != gpio374_value)
	{
		test_result = -81;
		printf("Walking zeros test failure on gpio379 to gpio374: %d\n", test_result);
	}
	if (gpio381_value != gpio375_value)
	{
		test_result = -82;
		printf("Walking zeros test failure on gpio381 to gpio375: %d\n", test_result);
	}
	if (gpio380_value != gpio377_value)
	{
		test_result = -83;
		printf("Walking zeros test failure on gpio380 to gpio377: %d\n", test_result);
	}
	if (gpio376_value != gpio378_value)
	{
		test_result = -84;
		printf("Walking zeros test failure on gpio376 to gpio378: %d\n", test_result);
	}

	// Close the GPIO value property files.
	fclose(fp_pmod_d0);
	fclose(fp_pmod_d1);
	fclose(fp_pmod_d2);
	fclose(fp_pmod_d3);
	fclose(fp_pmod_d4);
	fclose(fp_pmod_d5);
	fclose(fp_pmod_d6);
	fclose(fp_pmod_d7);

	return test_result;
}

int main()
{
	char gpio_setting[5];
	int test_result = 0;
	FILE  *fp;

	// Display the lab name in the application banner.
	printf("\n");
	printf("***********************************************************\n");
	printf("*                                                         *\n");
	printf("*          UltraZed-EG SOM + IO Carrier Card              *\n"); 
	printf("*                PS Pmod Loopback Test                    *\n");
	printf("*                                                         *\n");
	printf("* Pmod loopback cable must be installed on JPS1 connector *\n");
	printf("*                                                         *\n");
	printf("***********************************************************\n");
	printf("\n");
	
	// Open the export file and write the PSGPIO number for each Pmod GPIO
	// signal to the Linux sysfs GPIO export property, then close the file.
	fp = fopen("/sys/class/gpio/export", "w");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/export node\n");
	}
	else
	{
		// Set the export property for gpio379 - PMOD_D0.
		strcpy(gpio_setting, "379");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the export property for gpio381 - PMOD_D1.
		strcpy(gpio_setting, "381");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio380 - PMOD_D2.
		strcpy(gpio_setting, "380");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio376 - PMOD_D3.
		strcpy(gpio_setting, "376");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio374 - PMOD_D4.
		strcpy(gpio_setting, "374");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio375 - PMOD_D5.
		strcpy(gpio_setting, "375");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio377 - PMOD_D6.
		strcpy(gpio_setting, "377");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for gpio378 - PMOD_D7.
		strcpy(gpio_setting, "378");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D0.
	fp = fopen("/sys/class/gpio/gpio379/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio379/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D0 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 379 set as INPUT\n");
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO 379 set as OUTPUT\n");
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D1.
	fp = fopen("/sys/class/gpio/gpio381/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio381/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D1 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 381 set as INPUT\n");
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO 381 set as OUTPUT\n");
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D2.
	fp = fopen("/sys/class/gpio/gpio380/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio380/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D2 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 380 set as INPUT\n");
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO 380 set as OUTPUT\n");
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D3.
	fp = fopen("/sys/class/gpio/gpio376/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio376/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D3 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 376 set as INPUT\n");
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("GPIO 376 set as OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D4.
	fp = fopen("/sys/class/gpio/gpio374/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio374/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D4 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 374 set as INPUT\n");
		}
		else
		{
			printf("GPIO 374 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for PMOD_D5.
	fp = fopen("/sys/class/gpio/gpio375/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio375/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D5 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 375 set as INPUT\n");
		}
		else
		{
			printf("GPIO 375 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D6.
	fp = fopen("/sys/class/gpio/gpio377/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio377/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D6 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 377 set as INPUT\n");
		}
		else
		{
			printf("GPIO 377 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for PMOD_D7.
	fp = fopen("/sys/class/gpio/gpio378/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio378/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the PMOD_D7 GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("GPIO 378 set as INPUT\n");
		}
		else
		{
			printf("GPIO 378 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		fclose(fp);
	}

	// Perform a walking ones test on the Pmod GPIOs.
	printf("Walking ones test on PS Pmod JSP1\n");

	if (test_result == 0)
	{
		test_result = first_walking_ones_step();
	}

	if (test_result == 0)
	{
		test_result = second_walking_ones_step();
	}

	if (test_result == 0)
	{
		test_result = third_walking_ones_step();
	}

	if (test_result == 0)
	{
		test_result = fourth_walking_ones_step();
	}

	// Perform a walking zeros test on the Pmod GPIOs.
	printf("Walking zeros test on PS Pmod JSP1\n");

	if (test_result == 0)
	{
		test_result = first_walking_zeros_step();
	}

	if (test_result == 0)
	{
		test_result = second_walking_zeros_step();
	}

	if (test_result == 0)
	{
		test_result = third_walking_zeros_step();
	}

	if (test_result == 0)
	{
		test_result = fourth_walking_zeros_step();
	}

	printf("PS Pmod Loopback Tests complete...");
	
	if (test_result == 0)
	{
		printf("\033[32mPASSED\033[0m\n");
	}
	else
	{
		printf("\033[5mFAILED\033[0m\n");
	}

    exit(test_result);
}
