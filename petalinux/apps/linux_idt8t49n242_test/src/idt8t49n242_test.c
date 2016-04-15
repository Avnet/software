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
// Create Date:         Nov 27, 2015
// Design Name:         PicoZed FMC2 Carrier IIC Clock Synthesizer Test
// Module Name:         idt8t49n242_test.c
// Project Name:        PicoZed FMC2 Carrier IIC Clock Synthesizer Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         Clock Synthesizer test for PicoZed FMC2 Carrier
//
// Dependencies:
//
// Revision:            Nov 27, 2015: 1.00 Initial version
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include "platform.h"

#include "idt8t49n242_demo.h"

void print(const char *str);

// This is the top level demo instance.
iic_eeprom_demo_t demo;

int run_idt8t49N242_test(void)
{
	int ret;
	int pass_fail = 0;

	printf("\n\r");
	printf("---------------------------------------------------------------------------\n\r");
	printf("--                                                                       --\n\r");
	printf("--                PZCC-FMC2 Clock Generator Test Application             --\n\r");
	printf("--                                                                       --\n\r");
	printf("---------------------------------------------------------------------------\n\r");
	printf("\n\r");
    /* Assign the address for the IIC controller which comes from the address
     * map for hardware peripherals on this platform.
     */
    demo.uBaseAddr_IIC_RTC        = XPAR_AXI_IIC_0_BASEADDR;

    // Initialize hardware design and prepare devices for normal operation.
    ret = idt8t49N242_demo_init(&demo);
    if (ret != 0){
    	 printf("idt8t49N242_demo_init failed! Test ABORT!\n\r");
    	 pass_fail = -1;
    }

    ret = validate_idt8t49N242(&demo);
    if (ret != 0){
    	 printf("validate_idt8t49N242 failed! Test ABORT!\n\r");
    	 pass_fail = -1;
    }

    /* Determine overall test result and print results to test operator.
     */
    printf("\r\n");
    printf("Clock Generator Test: ");

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

    return pass_fail;
}
