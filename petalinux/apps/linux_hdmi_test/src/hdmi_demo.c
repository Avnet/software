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


int write_reg(iic_eeprom_demo_t *pDemo, uint8_t RegAddress, uint8_t RegData)
{
	int8u ChipAddress = HDMI_SLAVE_ADDRESS;
	int8u WriteData[0x02];
	int8u ByteCount = 0x01;
	int ret;

	// Power up the ADV7511.
	WriteData[0] = RegData;

	ret = pDemo->eeprom_iic.fpIicWrite(&(pDemo->eeprom_iic), ChipAddress, RegAddress, WriteData, ByteCount);
	if (!ret)
	{
		printf("ERROR : Failed to Write to HDMI Controller Reg 0x%02X\n\r", RegAddress);
		return -1;
	}

	return 0;
}


/*****************************************************************************
*
* Write the 720p configuration to the ADV7511 HDMI controller.
*
* @param	None.
*
* @return	0 if successful, else -1.
*
* @note		None.
*
*****************************************************************************/
int configure_hdmi(iic_eeprom_demo_t *pDemo)
{
    // Begin printing the content table to the terminal.
    printf("+-------------------------------------------------------------------------+\r\n");
    printf("|             Configuring HDMI Controller for 720p Output                 |\r\n");
    printf("|-------------------------------------------------------------------------|\r\n");

    // Power on the ADV7511 device.
    write_reg(pDemo, 0x41, 0x10);

	// Fulfill requirements of ADV7511 to set appropriate operation registers.
    write_reg(pDemo, 0x98, 0x03);
    write_reg(pDemo, 0x9A, 0xE0);
    write_reg(pDemo, 0x9C, 0x30);
    write_reg(pDemo, 0x9D, 0x61);
    write_reg(pDemo, 0xA2, 0xA4);
    write_reg(pDemo, 0xA3, 0xA4);
    write_reg(pDemo, 0xE0, 0xD0);
    write_reg(pDemo, 0x55, 0x12);
    write_reg(pDemo, 0xF9, 0x00);

    // Set input mode to YCbCr 422, DDR, Embedded Sync.
    write_reg(pDemo, 0x15, 0x08);
    // Set for left justified data.
    write_reg(pDemo, 0x48, 0x10);
    // Set for 444 output, 8-bit style 1, 1st half on rising edge - YCrCb clipping.
    write_reg(pDemo, 0x16, 0x3B);
    // Set output aspect ratio 4:3, external DE.
    write_reg(pDemo, 0x17, 0x00);
    // Set auto sync data for DDR modes with no DDR clock delay.
    write_reg(pDemo, 0xD0, 0x3C);

    // Set output mode to DVI mode.
    write_reg(pDemo, 0xAF, 0x04);
    // Set output mode to disable deep color mode.
	write_reg(pDemo, 0x4C, 0x04);
	// Set output mode to turn off additional data packets.
	write_reg(pDemo, 0x40, 0x00);

	// YCrCb --> RGB conversion, per programming guide and based upon
	// Table 57 values - HDTV YCbCr (16to 235) to RGB (0 to 255)
	write_reg(pDemo, 0x18, 0xE7);
	write_reg(pDemo, 0x19, 0x34);
	write_reg(pDemo, 0x1A, 0x04);
	write_reg(pDemo, 0x1B, 0xAD);
	write_reg(pDemo, 0x1C, 0x00);
	write_reg(pDemo, 0x1D, 0x00);
	write_reg(pDemo, 0x1E, 0x1C);
	write_reg(pDemo, 0x1F, 0x1B);
	write_reg(pDemo, 0x20, 0x1D);
	write_reg(pDemo, 0x21, 0xDC);
	write_reg(pDemo, 0x22, 0x04);
	write_reg(pDemo, 0x23, 0xAD);
	write_reg(pDemo, 0x24, 0x1F);
	write_reg(pDemo, 0x25, 0x24);
	write_reg(pDemo, 0x26, 0x01);
	write_reg(pDemo, 0x27, 0x35);
	write_reg(pDemo, 0x28, 0x00);
	write_reg(pDemo, 0x29, 0x00);
	write_reg(pDemo, 0x2A, 0x04);
	write_reg(pDemo, 0x2B, 0xAD);
	write_reg(pDemo, 0x2C, 0x08);
	write_reg(pDemo, 0x2D, 0x7C);
	write_reg(pDemo, 0x2E, 0x1B);
	write_reg(pDemo, 0x2F, 0x77);

	// Set for 720p embedded sync Hsync placement.
	write_reg(pDemo, 0x30, 0x1B);
	// Set for 720p embedded sync Hsync duration.
	write_reg(pDemo, 0x31, 0x82);
	write_reg(pDemo, 0x32, 0x80);
	// Set for 720p embedded sync Vsync placement.
	write_reg(pDemo, 0x33, 0x14);
	// Set for 720p embedded sync Vsync duration.
	write_reg(pDemo, 0x34, 0x05);
	write_reg(pDemo, 0x35, 0x40);
	write_reg(pDemo, 0x36, 0xD9);
	write_reg(pDemo, 0x37, 0x0A);
	write_reg(pDemo, 0x38, 0x00);
	write_reg(pDemo, 0x39, 0x2D);
	write_reg(pDemo, 0x3A, 0x00);
	write_reg(pDemo, 0xD7, 0x1B);
	write_reg(pDemo, 0xD8, 0x82);
	write_reg(pDemo, 0xD9, 0x80);
	write_reg(pDemo, 0xDA, 0x14);
	write_reg(pDemo, 0xDB, 0x05);

    // Finish printing the content table to the terminal.
    printf("|-------------------------------------------------------------------------|\r\n");
    printf("\r\n");

    return 0;
}


/*****************************************************************************
*
* Read the contents of the HDMI register space and print the chip ID to the
* terminal.
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
	int16u RegAddress = 0x00;
	int8u ReadData[0xFF];
	int8u ByteCount = 0xFF;
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

