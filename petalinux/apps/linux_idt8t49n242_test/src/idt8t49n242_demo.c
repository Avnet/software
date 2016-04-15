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
// Design Name:         PicoZed FMC2 IIC Clock Synthesizer Demonstration
// Module Name:         idt8t49n242_demo.c
// Project Name:        PicoZed FMC2 IIC Clock Synthesizer Demonstration
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

#include "idt8t49n242_demo.h"

#define IDT8T49N242_SLAVE_ADDRESS                    0x6C
#define IDT8T49N242_DEVICE_ID                        0x0607

int idt8t49N242_demo_init(iic_eeprom_demo_t *pDemo)
{
   int ret;

   /* IIC initialization for communicating with the RTC slave hardware.
    */
   printf("Clock Generator Initialization ...\n\r");
   ret = zed_iic_axi_init(&(pDemo->eeprom_iic),"RTC I2C", pDemo->uBaseAddr_IIC_RTC);
   if (!ret)
   {
      printf("ERROR : Failed to open AXI IIC device driver\n\r");
      return -1;
   }

   /* Check to see how the device has been configured. */
   //zed_rtc_demo_status(pDemo);

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
int validate_idt8t49N242(iic_eeprom_demo_t *pDemo)
{
	int8u ChipAddress = IDT8T49N242_SLAVE_ADDRESS;
	int16u RegAddress  = 0x0002;
	int8u ReadData[8];
	int8u ByteCount   = 0;
	int8u Rev_ID = 0;
	int16u Dev_ID = 0;
	int16u Dash_Code = 0;
	int ret;

    // Begin printing the content table to the terminal.
    printf("+-------------------------------------------------------------------------+\r\n");
    printf("|                       Validating IDT8T49N242                            |\r\n");
    printf("|-------------------------------------------------------------------------|\r\n");

    ReadData[0] = 0;
    ReadData[1] = 0;
    ReadData[2] = 0;
    ReadData[3] = 0;

    ByteCount   = 4;

    ret = pDemo->eeprom_iic.fpIicERead(&(pDemo->eeprom_iic), ChipAddress, RegAddress, ReadData, ByteCount);
	if (ret != ByteCount)
	{
		printf("ERROR : Failed to Read all %d bytes from IDT8T49N242\n\r", ByteCount);
		return -1;
	}

	Rev_ID = ReadData[0] >> 4;
	Dev_ID = (((int16u)ReadData[0]) << 12) | (((int16u)ReadData[1])<<4) | (((int16u)ReadData[2])>>4);
	Dash_Code = (((int16u)ReadData[2] & 0x0F)<<7) | (((int16u)ReadData[3])>>1);

/*	printf("Device Data: 0x%02X\t0x%02X\t0x%02X\t0x%02X\t\r\n",
						ReadData[0],
						ReadData[1],
						ReadData[2],
						ReadData[3]);*/

	printf("| Rev_ID: 0x%02X                                                            |\r\n",Rev_ID);
	printf("| Dev_ID: 0x%04X                                                          |\r\n",Dev_ID);
	printf("| Dash_Code: 0x%04X                                                       |\r\n",Dash_Code);

    // Finish printing the content table to the terminal.
	printf("|-------------------------------------------------------------------------|\r\n");
    printf("\r\n");

    /* Check to see if reported device ID matches the expected one for */
    if (Dev_ID == IDT8T49N242_DEVICE_ID)
    {
    	return 0;
    }

    /* The reported device ID did not match the expected one so the device
     * validation test fails. */
    return -1;
}

