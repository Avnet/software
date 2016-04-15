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
// Create Date:         Nov 30, 2015
// Design Name:         PicoZed FMC2 Carrier HDMI Test
// Module Name:         hdmi_demo.c
// Project Name:        PicoZed FMC2 Carrier HDMI Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         HDMI test for PicoZed FMC2 Carrier
//
// Dependencies:
//
// Revision:            Nov 30, 2015: 1.00 Initial version
//
//----------------------------------------------------------------------------

#include <stdio.h>

#include "hdmi_demo.h"


int hdmi_demo_init(iic_eeprom_demo_t *pDemo)
{
   int ret;

   /* IIC initialization for communicating with the RTC slave hardware.
    */
   printf("HDMI Controller Initialization ...\n\r");
   ret = zed_iic_axi_init(&(pDemo->eeprom_iic),"RTC I2C", pDemo->uBaseAddr_IIC_RTC);
   if (!ret)
   {
      printf("ERROR : Failed to open AXI IIC device driver\n\r");
      return -1;
   }

   return 0;
}


/*****************************************************************************
*
* Read the contents of the EEPROM memory and dump it to the terminal.
*
* @param	None.
*
* @return	0 if successful, else -1.
*
* @note		None.
*
*****************************************************************************/
int validate_hdmi(iic_eeprom_demo_t *pDemo)
{
	int8u ChipAddress = HDMI_SLAVE_ADDRESS;
	int16u RegAddress  = 0x00;
	int8u ReadData[0xFF];
	int8u ByteCount   = 0xFF;
	int8u Chip_Rev = 0;
	int16u Chip_ID = 0;


	int ret;

    // Begin printing the content table to the terminal.
    printf("+-------------------------------------------------------------------------+\r\n");
    printf("|                      Validating HDMI Controller                         |\r\n");
    printf("|-------------------------------------------------------------------------|\r\n");

    ReadData[0] = 0;
    ReadData[1] = 0;
    ReadData[2] = 0;
    ReadData[3] = 0;


    ret = pDemo->eeprom_iic.fpIicRead(&(pDemo->eeprom_iic), ChipAddress, RegAddress, ReadData, ByteCount);
	if (!ret)
	{
		printf("ERROR : Failed to Read from HDMI Controller\n\r");
		return -1;
	}

	Chip_Rev = ReadData[0];
    Chip_ID = (((int16u)ReadData[0xF5]<<8)|(ReadData[0xF6]));

	printf("| Chip_Rev: 0x%02X                                                          |\r\n",Chip_Rev);
	printf("| Chip_ID: 0x%04X                                                         |\r\n",Chip_ID);

    // Finish printing the content table to the terminal.
    printf("|-------------------------------------------------------------------------|\r\n");
    printf("\r\n");

    // Check to see if the data collected makes sense for the slave
    // we are testing for.
    if (Chip_ID == HDMI_CHIP_ID)
    {
    	return 0;
    }

    // If the correct Chip ID is not read, then the test failed.
    return -1;
}

