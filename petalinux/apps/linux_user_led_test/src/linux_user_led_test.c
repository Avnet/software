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
//                     Copyright(c) 2013 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Dec 04, 2013
// Design Name:         LED and PB test application for PicoZed FMC2 Carrier
// Module Name:         linux_user_led_test.c
// Project Name:        LED and PB test application for PicoZed FMC2 Carrier
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         User LED test application for Linux.
//
// Dependencies:
//
// Revision:            Dec 04, 2013: 1.00 Initial version
//                      Apr 06, 2016: 1.01 Updated to run under 2015.2
//                                         PetaLinux tools
//
//----------------------------------------------------------------------------


#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned int direction = 1;

int set_next_count_pattern(void)
{
	static unsigned int count = 0;
	char gpio_setting[5];
	int test_result = 0;

	FILE  *fp_led0;
	FILE  *fp_led1;
	FILE  *fp_led2;
	FILE  *fp_led3;

	// Open the gpio value properties so that they can be read/written.
	fp_led0 = fopen("/sys/class/gpio/gpio965/value", "r+");
	fp_led1 = fopen("/sys/class/gpio/gpio966/value", "r+");
	fp_led2 = fopen("/sys/class/gpio/gpio967/value", "r+");
	fp_led3 = fopen("/sys/class/gpio/gpio968/value", "r+");

	// Write test pattern to LEDs.
	if ((count == 0) && ((direction == 0) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");

		if (direction == 0)
		{
			// Now begin sliding 'up'.
			direction = 1;
		}
	}
	else if (((count & 0x0001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	fwrite(&gpio_setting, sizeof(char), 1, fp_led0);
	fflush(fp_led0);

	if ((count == 1) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 1) & 0x0001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	fwrite(&gpio_setting, sizeof(char), 1, fp_led1);
	fflush(fp_led1);

	if ((count == 2) && ((direction == 0) || (direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");
	}
	else if ((((count >> 2) & 0x0001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	fwrite(&gpio_setting, sizeof(char), 1, fp_led2);
	fflush(fp_led2);

	if ((count == 3) && ((direction == 1) || (direction == 4) || (direction == 5)))
	{
		strcpy(gpio_setting, "1");

		if (direction == 1)
		{
			// Last LED in the set, begin sliding 'down'.
			direction = 0;
		}
	}
	else if ((((count >> 3) & 0x0001) == 1) && ((direction == 2) || (direction == 3)))
	{
		strcpy(gpio_setting, "1");
	}
	else
	{
		strcpy(gpio_setting, "0");
	}
	fwrite(&gpio_setting, sizeof(char), 1, fp_led3);
	fflush(fp_led3);

	if (((direction == 1) & (count < 4)) ||
		(direction == 2))
	{
		// Increment count for next time around.
		count = count + 1;
	}
	else if ((direction == 0) ||
	         (direction == 3))
	{
		// Decrement count for next time around.
		count = count - 1;
	}
	else if (direction == 4)
	{
		// Increment count for next time around.
		if (count == 3)
		{
			count = 0;
		}
		else
		{
			count = count + 1;
		}
	}
	else if (direction == 5)
	{
		// Decrement count for next time around.
		if (count == 0)
		{
			count = 3;
		}
		else
		{
			count = count - 1;
		}
	}
	else
	{
		/*
		 *  Something went wrong keeping track of direction, reset the
		 *  direction to 'up'.
		 */
		direction = 1;
		count = 0;
	}

	// Close the GPIO value property files.
	fclose(fp_led0);
	fclose(fp_led1);
	fclose(fp_led2);
	fclose(fp_led3);

	return test_result;
}

int set_next_input_pattern(void)
{
	char gpio_setting[5];
	int test_result = 0;

	FILE  *fp_led0;
	FILE  *fp_led1;
	FILE  *fp_led2;
	FILE  *fp_led3;
	FILE  *fp_pb0;
	FILE  *fp_pb1;
	FILE  *fp_pb2;
	FILE  *fp_pb3;
	FILE  *fp_pb4;

	// Open the gpio value properties so that they can be read/written.
	fp_led0 = fopen("/sys/class/gpio/gpio965/value", "r+");
	fp_led1 = fopen("/sys/class/gpio/gpio966/value", "r+");
	fp_led2 = fopen("/sys/class/gpio/gpio967/value", "r+");
	fp_led3 = fopen("/sys/class/gpio/gpio968/value", "r+");
	fp_pb0 = fopen("/sys/class/gpio/gpio960/value", "r");
	fp_pb1 = fopen("/sys/class/gpio/gpio961/value", "r");
	fp_pb2 = fopen("/sys/class/gpio/gpio962/value", "r");
	fp_pb3 = fopen("/sys/class/gpio/gpio963/value", "r");
	fp_pb4 = fopen("/sys/class/gpio/gpio964/value", "r");

	// Read the current value of the PB0 GPIO input.
	fscanf(fp_pb0, "%s", gpio_setting);

	// Determine whether the 'N' push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Count LEDs up.
		direction = 2;

		fflush(fp_led0);
		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
	}

	// Read the current value of the PB1 GPIO input.
	fscanf(fp_pb1, "%s", gpio_setting);

	// Determine whether the 'S' push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Count LEDs down.
		direction = 3;

		fflush(fp_led0);
		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
	}

	// Read the current value of the PB2 GPIO input.
	fscanf(fp_pb2, "%s", gpio_setting);

	// Determine whether the 'E' push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Slide LED to the right.
		direction = 4;

		fflush(fp_led0);
		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
	}

	// Read the current value of the PB3 GPIO input.
	fscanf(fp_pb3, "%s", gpio_setting);

	// Determine whether the 'W' push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Slide LED to the left.
		direction = 5;

		fflush(fp_led0);
		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
	}

	// Read the current value of the PB4 GPIO input.
	fscanf(fp_pb4, "%s", gpio_setting);

	// Determine whether the 'C' push button is being depressed or not.
	if (!strcmp(gpio_setting, "1"))
	{
		// Write test pattern to LEDs.
		if (direction == 0)
		{
			strcpy(gpio_setting, "1");

			// Now turn all LEDs ON.
			direction = 1;
		}
		else
		{
			strcpy(gpio_setting, "0");

			// Now turn all LEDs OFF.
			direction = 0;
		}
		fwrite(&gpio_setting, sizeof(char), 1, fp_led0);
		fwrite(&gpio_setting, sizeof(char), 1, fp_led1);
		fwrite(&gpio_setting, sizeof(char), 1, fp_led2);
		fwrite(&gpio_setting, sizeof(char), 1, fp_led3);
		fflush(fp_led0);
		fflush(fp_led1);
		fflush(fp_led2);
		fflush(fp_led3);
	}

	// Close the GPIO value property files.
	fclose(fp_led0);
	fclose(fp_led1);
	fclose(fp_led2);
	fclose(fp_led3);
	fclose(fp_pb0);
	fclose(fp_pb1);
	fclose(fp_pb2);
	fclose(fp_pb3);
	fclose(fp_pb4);

	return test_result;
}

int main()
{
	char gpio_setting[5];
	int test_result = 0;
	FILE  *fp;

	// Display the lab name in the application banner.
	printf("*********************************************************\n");
	printf("*                                                       *\n");
	printf("*   MicroZed IO Carrier LED Tests                       *\n");
	printf("*                                                       *\n");
	printf("*********************************************************\n");

	// Open the export file and write the PSGPIO number for each Pmod GPIO
	// signal to the Linux sysfs GPIO export property, then close the file.
	fp = fopen("/sys/class/gpio/export", "w");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/export node\n");
	}
	else
	{
		// Set the export property for GPIO54, which is gpio960 under Linux.
		strcpy(gpio_setting, "960");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the export property for GPIO55, which is gpio961 under Linux.
		strcpy(gpio_setting, "961");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for GPIO56, which is gpio962 under Linux.
		strcpy(gpio_setting, "962");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for GPIO57, which is gpio963 under Linux.
		strcpy(gpio_setting, "963");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);
		
		// Set the export property for GPIO58, which is gpio964 under Linux.
		strcpy(gpio_setting, "964");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the export property for GPIO59, which is gpio965 under Linux.
		strcpy(gpio_setting, "965");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the export property for GPIO59, which is gpio966 under Linux.
		strcpy(gpio_setting, "966");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the export property for GPIO59, which is gpio967 under Linux.
		strcpy(gpio_setting, "967");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		// Set the export property for GPIO59, which is gpio968 under Linux.
		strcpy(gpio_setting, "968");
		fwrite(&gpio_setting, sizeof(char), 3, fp);
		fflush(fp);

		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO54.
	fp = fopen("/sys/class/gpio/gpio960/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio960/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("gpio960 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio960 set as INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO55.
	fp = fopen("/sys/class/gpio/gpio961/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio961/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("gpio961 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio961 set as INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO56.
	fp = fopen("/sys/class/gpio/gpio962/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio962/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("gpio962 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio962 set as INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO57.
	fp = fopen("/sys/class/gpio/gpio963/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio963/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("gpio963 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio963 set as INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO58.
	fp = fopen("/sys/class/gpio/gpio964/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio964/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "out"))
		{
			printf("gpio964 set as OUTPUT\n");

			// Set the direction property to "in".
			strcpy(gpio_setting, "in");
			fwrite(&gpio_setting, sizeof(char), 2, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio964 set as INPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO59.
	fp = fopen("/sys/class/gpio/gpio965/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio965/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("gpio965 set as INPUT\n");
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio965 set as OUTPUT\n");
		}
		fclose(fp);
	}
	
	// Check the direction property of the PSGPIO number for GPIO59.
	fp = fopen("/sys/class/gpio/gpio966/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio966/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("gpio966 set as INPUT\n");
			
			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio965 set as OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO60.
	fp = fopen("/sys/class/gpio/gpio967/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio967/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("gpio967 set as INPUT\n");

			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio967 set as OUTPUT\n");
		}
		fclose(fp);
	}

	// Check the direction property of the PSGPIO number for GPIO61.
	fp = fopen("/sys/class/gpio/gpio968/direction", "r+");
	if (fp == NULL)
	{
		printf("Error opening /sys/class/gpio/gpio968/direction node\n");
	}
	else
	{
		fscanf(fp, "%s", gpio_setting);

		// Display whether the GPIO is set as input or output.
		if (!strcmp(gpio_setting, "in"))
		{
			printf("gpio968 set as INPUT\n");

			// Set the direction property to "out".
			strcpy(gpio_setting, "out");
			fwrite(&gpio_setting, sizeof(char), 3, fp);
			fflush(fp);
		}
		else
		{
			printf("gpio968 set as OUTPUT\n");
		}
		fclose(fp);
	}

	// Perform LED pattern generation.
	printf("LED Pattern Generation on PicoZed FMC2 Carrier\n");

	while (test_result == 0)
	{
		test_result = set_next_count_pattern();
		test_result = set_next_input_pattern();
		usleep(110000);
	}

	printf("LED Pattern Generation complete...");
	
	if (test_result == 0)
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}

    exit(test_result);
}
