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
// Create Date:         Dec 05, 2013
// Design Name:         PicoZed FMC2 FMC Loopback Test
// Module Name:         fmc_lpc.c
// Project Name:        PicoZed FMC2 FMC Loopback Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         FMC LPC loopback test routines.
//
// Dependencies:
//
// Revision:            Dec 05, 2013: 1.00 Initial version
//
//----------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// System includes.
#include <stdio.h>

// ---------------------------------------------------------------------------
// Local includes.
#include "fmc_lpc.h"

/************************** Variable Definitions *****************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

int32u fmc1_lpc_high; /* The Instance of the GPIO for the FMC1 high LA pair loopback instance. */
int32u fmc1_lpc_low; /* The Instance of the GPIO for the FMC1 low LA pair loopback instance. */

// ----------------------------------------------------------------
int fmc_lpc_setup(int32u verbosity)
{
	int final_result = TEST_SUCCESS;
	int unit_result = TEST_SUCCESS;

	/*
	 * Initialize the GPIO driver instances.
	 */
	unit_result = pl_gpio_initialize(&fmc1_lpc_high, FMC1_LPC_SE_HIGH_DEVICE_ADDRESS);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}
	else
	{
		if (verbosity)
		{
			printf("   Mapped Pmod Single 1 successfully to 0x%08X\r\n", fmc1_lpc_high);
		}
	}

	unit_result = pl_gpio_initialize(&fmc1_lpc_low, FMC1_LPC_SE_LOW_DEVICE_ADDRESS);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}
	else
	{
		if (verbosity)
		{
			printf("   Mapped FMC1 SE LOW successfully to 0x%08X\r\n", fmc1_lpc_low);
		}
	}

	return final_result;
}

// ---------------------------------------------------------------------------
int fmc1_lpc_high_test(int32u test_pattern, int32u verbosity)
{
	int result = TEST_SUCCESS;
	int32u delay_wait = 1;
    int32u test_pattern_channel1;
    int32u test_pattern_channel2;

    /*
     * Set the direction for both of the channel1 IOs to output.
     */
    if (pl_gpio_set_data_direction(fmc1_lpc_high, PL_GPIO_CHANNEL1, (FMC_LPC_BIT_MASK & ~FMC_LPC_BIT_MASK)) != TEST_SUCCESS)
    {
    	printf("   Can't set FMC1 LPC High, GPIO channel 1 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the direction for both of the channel2 IOs to input.
     */
    if (pl_gpio_set_data_direction(fmc1_lpc_high, PL_GPIO_CHANNEL2, FMC_LPC_BIT_MASK) != TEST_SUCCESS)
    {
    	printf("   Can't set FMC1 LPC High, GPIO channel 2 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the values of each of the output channels.
     */
    test_pattern_channel1 = (test_pattern & FMC_LPC_BIT_MASK);
    if (pl_gpio_data_write(fmc1_lpc_high, PL_GPIO_CHANNEL1, &test_pattern_channel1) != TEST_SUCCESS)
    {
    	printf("   Can't write data to FMC1 LPC High GPIO\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Wait for some time to get the loopback propagation latched.
     */
    while (delay_wait-- > 0);

    /*
     * Get the values of each of the input channels.
     */
    if (pl_gpio_data_read(fmc1_lpc_high, PL_GPIO_CHANNEL2, &test_pattern_channel2) != TEST_SUCCESS)
    {
    	printf("   Can't read data from FMC1 LPC High GPIO\r\n");
    	result = TEST_FAILURE;
    }
    test_pattern_channel2 = (test_pattern_channel2 & FMC_LPC_BIT_MASK);

    /*
     * Compare the values of each of the result registers to determine if the
     * test pattern propagated successfully.
     */
    if (test_pattern_channel1 != test_pattern_channel2)
    {
        printf("   \033[5mFAILURE\033[0m on FMC1 LPC High: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);

        result = TEST_FAILURE;
    }
    else if (verbosity)
    {
        printf("   SUCCESS on FMC1 LPC High: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);
    }

    return result;
}

// ---------------------------------------------------------------------------
int fmc1_lpc_low_test(int32u test_pattern, int32u verbosity)
{
	int result = TEST_SUCCESS;
	int32u delay_wait = 1;
    int32u test_pattern_channel1;
    int32u test_pattern_channel2;

    /*
     * Set the direction for both of the channel1 IOs to output.
     */
    if (pl_gpio_set_data_direction(fmc1_lpc_low, PL_GPIO_CHANNEL1, (FMC_LPC_BIT_MASK & ~FMC_LPC_BIT_MASK)) != TEST_SUCCESS)
    {
    	printf("   Can't set FMC1 LPC Low, GPIO channel 1 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the direction for both of the channel2 IOs to input.
     */
    if (pl_gpio_set_data_direction(fmc1_lpc_low, PL_GPIO_CHANNEL2, FMC_LPC_BIT_MASK) != TEST_SUCCESS)
    {
    	printf("   Can't set FMC1 LPC Low, GPIO channel 2 direction\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Set the values of each of the output channels.
     */
    test_pattern_channel1 = (test_pattern & FMC_LPC_BIT_MASK);
    if (pl_gpio_data_write(fmc1_lpc_low, PL_GPIO_CHANNEL1, &test_pattern_channel1) != TEST_SUCCESS)
    {
    	printf("   Can't write data to FMC1 LPC Low GPIO\r\n");
    	result = TEST_FAILURE;
    }

    /*
     * Wait for some time to get the loopback propagation latched.
     */
    while (delay_wait-- > 0);

    /*
     * Get the values of each of the input channels.
     */
    if (pl_gpio_data_read(fmc1_lpc_low, PL_GPIO_CHANNEL2, &test_pattern_channel2) != TEST_SUCCESS)
    {
    	printf("   Can't read data from FMC1 LPC Low GPIO\r\n");
    	result = TEST_FAILURE;
    }
    test_pattern_channel2 = (test_pattern_channel2 & FMC_LPC_BIT_MASK);

    /*
     * Compare the values of each of the result registers to determine if the
     * test pattern propagated successfully.
     */
    if (test_pattern_channel1 != test_pattern_channel2)
    {
        printf("   \033[5mFAILURE\033[0m on FMC1 LPC Low: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);

        result = TEST_FAILURE;
    }
    else if (verbosity)
    {
        printf("   SUCCESS on FMC1 LPC Low: expected 0x%08X, actual 0x%08X\r\n", test_pattern_channel1, test_pattern_channel2);
    }

    return result;
}

// ---------------------------------------------------------------------------
int fmc_lpc_test(int32u test_pattern, int32u verbosity)
{
	int result = TEST_SUCCESS;
	int32u fail = 0;

	/*
	 * Call the individual tests for each of the FMC test blocks.
	 */
	if (fmc1_lpc_high_test(test_pattern, verbosity) != TEST_SUCCESS)
	{
		fail = 1;
	}

	if (fmc1_lpc_low_test(test_pattern, verbosity) != TEST_SUCCESS)
	{
		fail = 1;
	}

	if (fail == 0)
	{
		return result;
	}

	return TEST_FAILURE;
}


// ---------------------------------------------------------------------------
int fmc_lpc_walking_ones(int32u verbosity)
{
    int bit_under_test;
    int32u fail = 0;
    int32u wones = 0x00000001;
    int result = TEST_SUCCESS;
    
    for (bit_under_test = 0; bit_under_test < FMC_LPC_BIT_WIDTH; bit_under_test++)
    {
		// Perform the test result comparison.
        result = fmc_lpc_test(wones, verbosity);
        
        if (result == TEST_FAILURE)
        {
            fail = 1;
        }

		// If this is a deep debug mode, prompt the operator before continuing
		// the test sequence.
		if (verbosity > 1)
		{
			printf("Bits currently under test: %d\r\n\r\n", bit_under_test);
		}
		
        // Shift the one in the test pattern to prepare for the next test 
        // iteration.
        wones = wones << 1;
    }
    
    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("Walking ones FMC LPC Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }
    
    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}


// ---------------------------------------------------------------------------
int fmc_lpc_walking_zeros(int32u verbosity)
{
    int bit_under_test;
    int32u fail = 0;
    int32u wzeros = 0xFFFFFFFE;
    int result = TEST_SUCCESS;
    
    for (bit_under_test = 0; bit_under_test < FMC_LPC_BIT_WIDTH; bit_under_test++)
    {   
        // Perform the test result comparison.
        result = fmc_lpc_test(wzeros, verbosity);
        
        if (result == TEST_FAILURE)
        {
            fail = 1;
        }

		// If this is a deep debug mode, prompt the operator before continuing
		// the test sequence.
		if (verbosity > 1)
		{
			printf("Bits currently under test: %d\r\n\r\n", bit_under_test);
		}
		
        // Shift the zero in the test pattern to prepare for the next test 
        // iteration.
        wzeros = ~(wzeros);
        wzeros = wzeros << 1;
        wzeros = ~(wzeros);        
    }

    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("Walking zeros FMC LPC Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }

    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}

// ---------------------------------------------------------------------------
int fmc_lpc_all_zeros(int32u verbosity)
{
    int32u fail = 0;
    int32u zeros = 0x00000000;
    int result = TEST_SUCCESS;
    
    // Perform the test result comparison.
    result = fmc_lpc_test(zeros, verbosity);
    
    if (result == TEST_FAILURE)
    {
        fail = 1;
    }
    
    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("All zeros FMC LPC Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }

    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}


// ---------------------------------------------------------------------------
int fmc_lpc_all_ones(int32u verbosity)
{
    int32u fail = 0;
    int32u ones = 0xFFFFFFFF;
    int result = TEST_SUCCESS;
    
    // Perform the test result comparison.
    result = fmc_lpc_test(ones, verbosity);
    
    if (result == TEST_FAILURE)
    {
        fail = 1;
    }
    
    // Show the result of the test as either pass or fail.
    if (verbosity > 0)
	{
		printf("All ones FMC LPC Test...");
	}
    
    if (fail == 0)
    {
        if (verbosity > 0)
        {
            printf("\033[32mPASSED\033[0m\r\n");
        }
        
        return result;
    }

    if (verbosity > 0)
    {
        printf("\033[5mFAILED\033[0m\r\n");
    }
    
    return TEST_FAILURE;
}

