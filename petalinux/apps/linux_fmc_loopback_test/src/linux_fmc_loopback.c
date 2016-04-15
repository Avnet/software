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
// Create Date:         Jun 24, 2013
// Design Name:         PicoZed FMC2 FMC Loopback Test
// Module Name:         linux_fmc_loopback.c
// Project Name:        PicoZed FMC2 FMC Loopback Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         Dual Pmod pair loopback test Linux application.
//
// Dependencies:
//
// Revision:            Jun 24, 2013: 1.00 Initial version
//
//----------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// System includes.
#include <stdio.h>

// ---------------------------------------------------------------------------
// Local includes.
#include "fmc_lpc.h"
#include "platform.h"
#include "types.h"

int main()
{
	int32u verbosity = 1;
	int final_result = TEST_SUCCESS;
	int unit_result = TEST_SUCCESS;

	final_result = TEST_SUCCESS;
	unit_result = TEST_SUCCESS;

	/*
	 * Setup for the FMC LPC test by initializing the corresponding GPIO
	 * peripherals.
	 */
	unit_result = fmc_lpc_setup(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the FMC LPC all zeros test, this rules out any stuck at 0
	 * pins.
	 */
	unit_result = fmc_lpc_all_zeros(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the FMC LPC all ones test, this rules out any stuck at 1
	 * pins.
	 */
	unit_result = fmc_lpc_all_ones(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the FMC LPC walking zeros test, this rules out any
	 * unexpected shorted together nets.
	 */
	unit_result = fmc_lpc_walking_zeros(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Perform the FMC LPC walking ones test, this rules out any
	 * unexpected shorted together nets.
	 */
	unit_result = fmc_lpc_walking_ones(verbosity);
	if (unit_result != TEST_SUCCESS)
	{
		final_result = TEST_FAILURE;
	}

	/*
	 * Determine overall test result and print results to test operator.
	 */
	printf("FMC LPC Loopback Test: ");

	if (final_result == TEST_SUCCESS)
	{
		printf("\033[32mPASSED\033[0m\r\n");
	}
	else
	{
		printf("\033[5mFAILED\033[0m\r\n");
	}

    return final_result;
}
