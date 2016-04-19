# ----------------------------------------------------------------------------
#       _____
#      *     *
#     *____   *____
#    * *===*   *==*
#   *___*===*___**  AVNET
#        *======*
#         *====*
# ----------------------------------------------------------------------------
# 
#  This design is the property of Avnet.  Publication of this
#  design is not authorized without written consent from Avnet.
# 
#  Please direct any questions to the PicoZed community support forum:
#     http://www.picozed.org/forum
# 
#  Product information is available at:
#     http://www.picozed.org/product/picozed
# 
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2016 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Apr 05, 2016
#  Design Name:         PicoZed + FMC2 Carrier
#  Module Name:         factory_test.sh
#  Project Name:        PicoZed FMC2 Carrier HW Factory Acceptance Test Script
#  Target Devices:      Xilinx Zynq-7000
#  Hardware Boards:     PicoZed + FMC Carrier 2
# 
#  Tool versions:       Xilinx Vivado 2015.2
# 
#  Description:         Startup Script for PicoZed FMC2 HW Factory Acceptance 
#                       Test design
# 
#  Dependencies:        
#
#  Revision:            Apr 05, 2016: 1.00 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# Information about where test applications are located and what they are named:
TEST_APP_FOLDER=/home/root
FMC_LOOPBACK_TEST_APP=linux_fmc_loopback_test.elf
HDMI_TEST_APP=linux_hdmi_test.elf
IDT8T49N242_TEST_APP=linux_idt8t49n242_test.elf
IIC_EEPROM_TEST_APP=linux_iic_eeprom_test.elf
IIC_RTC_TEST_APP=linux_iic_rtc_test.elf
MAC_EEPROM_TEST_APP=linux_mac_eeprom_test.elf
USER_LED_TEST_APP=linux_user_led_test.elf

# Setup variables to store individual test results into.
FMC_LOOPBACK_TEST_RESULT=-1
HDMI_TEST_RESULT=-1
IDT8T49N242_TEST_RESULT=-1
IIC_EEPROM_TEST_RESULT=-1
IIC_RTC_TEST_RESULT=-1
MAC_EEPROM_TEST_RESULT=-1

	echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** STARTING FACTORY TEST OF ALL PERIPHERALS                   ***"
	echo "***                                                            ***"
	echo "******************************************************************"
	echo " "

# Launch the User LED and Push Button test.
if [ -e ${TEST_APP_FOLDER}/${FMC_LOOPBACK_TEST_APP} ]
then
	if [ -f ${TEST_APP_FOLDER}/${USER_LED_TEST_APP} ]
	then
		echo " "
		echo "+++ Launching User LED and Push Button Test..."
		echo " "
	    	${TEST_APP_FOLDER}/${USER_LED_TEST_APP}&
	fi
else
	echo " "
	echo "The File Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
	echo " "
fi

sleep 1
# Launch the FMC loopback test.
if [ -e ${TEST_APP_FOLDER}/${FMC_LOOPBACK_TEST_APP} ]
then
	if [ -f ${TEST_APP_FOLDER}/${FMC_LOOPBACK_TEST_APP} ]
	then
		echo " "
		echo "+++ Running FMC Loopback Test..."
		echo " "
	    	${TEST_APP_FOLDER}/${FMC_LOOPBACK_TEST_APP}
		FMC_LOOPBACK_TEST_RESULT=$?
	fi
else
	echo " "
	echo "The File Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
	echo " "
fi

sleep 1
if [ $FMC_LOOPBACK_TEST_RESULT == "0" ]; 
then
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** FMC/PMOD LOOPBACK:     PASS                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
else
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** FMC/PMOD LOOPBACK:     FAIL                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
fi

sleep 3
# Launch the HDMI test.
if [ -e ${TEST_APP_FOLDER}/${HDMI_TEST_APP} ]
then
	if [ -f ${TEST_APP_FOLDER}/${HDMI_TEST_APP} ]
	then
		echo " "
		echo "+++ Running HDMI Test..."
		echo " "
	    	${TEST_APP_FOLDER}/${HDMI_TEST_APP}
		HDMI_TEST_RESULT=$?
	fi
else
	echo " "
	echo "The File Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
	echo " "
fi

sleep 1
if [ $HDMI_TEST_RESULT == "0" ]; 
then
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** HDMI TEST:                     PASS                        ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
else
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** HDMI TEST:                     FAIL                        ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
fi

sleep 3
	# Launch the IDT8T49N242 test.
if [ -e ${TEST_APP_FOLDER}/${IDT8T49N242_TEST_APP} ]
then
	if [ -f ${TEST_APP_FOLDER}/${IDT8T49N242_TEST_APP} ]
	then
		echo " "
		echo "+++ Running IDT8T49N242 Test..."
		echo " "
	    	${TEST_APP_FOLDER}/${IDT8T49N242_TEST_APP}
		IDT8T49N242_TEST_RESULT=$?
	fi
else
	echo " "
	echo "The File Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
	echo " "
fi

sleep 1
if [ $IDT8T49N242_TEST_RESULT == "0" ]; 
then
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** IDT8T49N242 TEST:      PASS                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
else
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** IDT8T49N242 TEST:      FAIL                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
fi

sleep 3
	# Launch the FMC loopback test.
if [ -e ${TEST_APP_FOLDER}/${IIC_EEPROM_TEST_APP} ]
then
	if [ -f ${TEST_APP_FOLDER}/${IIC_EEPROM_TEST_APP} ]
	then
		echo " "
		echo "+++ Running IIC EEPROM Test..."
		echo " "
	    	${TEST_APP_FOLDER}/${IIC_EEPROM_TEST_APP}
		IIC_EEPROM_TEST_RESULT=$?
	fi
else
	echo " "
	echo "The File Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
	echo " "
fi

sleep 1
if [ $IIC_EEPROM_TEST_RESULT == "0" ]; 
then
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** IIC EEPROM TEST:       PASS                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
else
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** IIC EEPROM TEST:       FAIL                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
fi

sleep 3
	# Launch the Real Time Clock test.
if [ -e ${TEST_APP_FOLDER}/${IIC_RTC_TEST_APP} ]
then
	if [ -f ${TEST_APP_FOLDER}/${IIC_RTC_TEST_APP} ]
	then
		echo " "
		echo "+++ Running IIC RTC Test..."
		echo " "
	    	${TEST_APP_FOLDER}/${IIC_RTC_TEST_APP}
		IIC_RTC_TEST_RESULT=$?
	fi
else
	echo " "
	echo "The File Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
	echo " "
fi

sleep 1
if [ $IIC_RTC_TEST_RESULT == "0" ]; 
then
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** IIC RTC TEST:          PASS                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
else
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** IIC RTC TEST:          FAIL                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
fi

sleep 3
	# Launch the MAC ID EEPROM test.
if [ -e ${TEST_APP_FOLDER}/${MAC_EEPROM_TEST_APP} ]
then
	if [ -f ${TEST_APP_FOLDER}/${MAC_EEPROM_TEST_APP} ]
	then
		echo " "
		echo "+++ Running MAC EEPROM Test..."
		echo " "
	    	${TEST_APP_FOLDER}/${MAC_EEPROM_TEST_APP}
		MAC_EEPROM_TEST_RESULT=$?
	fi
else
	echo " "
	echo "The File Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
	echo " "
fi

sleep 1
if [ $MAC_EEPROM_TEST_RESULT == "0" ]; 
then
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** MAC EEPROM TEST:       PASS                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
else
        echo " "
	echo "******************************************************************"
	echo "***                                                            ***"                
	echo "*** MAC EEPROM TEST:       FAIL                                ***"
	echo "***                                                            ***"
	echo "******************************************************************"
        echo " "
fi

sleep 3
# Display the final results for all tests.
	echo " "
	echo "******************************************************************"
	echo "***                                                            ***"
	echo "***   PicoZed 7020 + FMC2 Factory Test V1.0 Complete           ***"
	echo "***                                                            ***"

if [ $FMC_LOOPBACK_TEST_RESULT == "0" ]; 
then
	echo "*** FMC LPC Loopback:     PASS                                 ***"
else
	echo "*** FMC LPC Loopback:     FAIL                                 ***"
fi

if [ $HDMI_TEST_RESULT == "0" ];
then
	echo "*** HDMI TEST:            PASS                                 ***"
else
	echo "*** HDMI TEST:            FAIL                                 ***"
fi

if [ $IDT8T49N242_TEST_RESULT == "0" ]; 		
then
	echo "*** IDT8T49N242 TEST:     PASS                                 ***"
else
	echo "*** IDT8T49N242 TEST:     FAIL                                 ***"
fi

if [ $IIC_EEPROM_TEST_RESULT == "0" ]; 		
then
	echo "*** IIC EEPROM TEST:      PASS                                 ***"
else
	echo "*** IIC EEPROM TEST:      FAIL                                 ***"
fi

if [ $IIC_RTC_TEST_RESULT == "0" ]; 		
then
	echo "*** IIC RTC TEST:         PASS                                 ***"
else
	echo "*** IIC RTC TEST:         FAIL                                 ***"
fi

if [ $MAC_EEPROM_TEST_RESULT == "0" ]; 		
then
	echo "*** MAC EEPROM TEST:      PASS                                 ***"
else
	echo "*** MAC EEPROM TEST:      FAIL                                 ***"
fi

	echo "***                                                            ***"
	echo "******************************************************************"
	echo " "


