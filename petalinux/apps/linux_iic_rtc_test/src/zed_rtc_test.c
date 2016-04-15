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
//                     Copyright(c) 2015 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Jul 21, 2015
// Design Name:         PicoZed FMC2 Real Time Clock Test
// Module Name:         zed_rtc_test.c
// Project Name:        PicoZed FMC2 Real Time Clock Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         Real Time Clock Test for PicoZed FMC2 Carrier
//
// Dependencies:
//
// Revision:            Jul 21, 2015: 1.00 Initial version
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include "platform.h"

#include "zed_rtc_demo.h"

void print(const char *str);

// This is the top level demo instance.
zed_rtc_demo_t demo;

int run_rtc_test()
{
	int pass_fail = 0;
	int ret;

    printf("\n\r");

    printf("---------------------------------------------------------------------------\n\r");
    printf("--                                                                       --\n\r");
    printf("--                PZCC-FMC2 Real Time Clock Test Application             --\n\r");
    printf("--                                                                       --\n\r");
    printf("---------------------------------------------------------------------------\n\r");
    printf("\n\r");

    /* Assign the address for the IIC controller which comes from the address
     * map for hardware peripherals on this platform.
     */
    demo.uBaseAddr_IIC_RTC        = XPAR_AXI_IIC_0_BASEADDR;

        // Initialize event and time tracking variables.
    demo.rtc_date = 0;
    demo.rtc_day = 0;
    demo.rtc_events = 0;
    demo.rtc_hours = 0;
    demo.rtc_irqs = 0;
    demo.rtc_minutes = 0;
    demo.rtc_month = 0;
    demo.rtc_seconds = 0;
    demo.rtc_status = 0;
    demo.rtc_temp_lower = 0;
    demo.rtc_temp_upper = 0;
    demo.rtc_temp = 0;
    demo.rtc_year = 0;

    // Initialize hardware design and prepare devices for normal operation.
    ret = zed_rtc_demo_init(&demo);
    if (ret != 0){
    	 printf("zed_rtc_demo_init failed! Test ABORT!\n\r");
		 pass_fail = -1;
	}

    printf("\r\n");
    printf("RTC Test: ");

    if (pass_fail == 0)
	{
		printf("\033[32mPASSED\033[0m\r\n");
		return 0;
	}
	else
	{
		printf("\033[5mFAILED\033[0m\r\n");
		return -1;
	}

    return 0;
}
