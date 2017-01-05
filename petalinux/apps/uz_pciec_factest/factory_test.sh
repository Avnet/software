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
#  Please direct any questions to the UltraZed community support forum:
#     http://www.ultrazed.org/forum
# 
#  Product information is available at:
#     http://www.ultrazed.org/product/ultrazed-EG
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
#  Create Date:         Dec 30, 2016
#  Design Name:         UltraZed + PCIe Carrier
#  Module Name:         factory_test.sh
#  Project Name:        UltraZed PCIe Carrier Factory Acceptance Test Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + PCIe Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         Startup Script for UltraZed PCIe Carrier Factory  
#                       Acceptance Test design
# 
#  Dependencies:        
#
#  Revision:            Dec 30, 2016: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# FAT version information.
FAT_VERSION_NUMBER=1.1

# Sleep interval between tests.
SLEEP_INTERVAL_NORMAL=1

# Information used to program Out Of Box components.
TEST_APP_FOLDER=/home/root
OOB_IMAGE_FOLDER=/run/media/mmcblk1
OOB_IMAGE_PRI_FOLDER=/run/media/mmcblk1
OOB_IMAGE_ALT_FOLDER=/run/media/mmcblk1p1
OOB_BOOT_FIRMWARE=BOOT_EMMC_OOB.bin
OOB_LINUX_IMAGE=image.oob.ub

# Variables used to determine whether a SOM is being tested or if a Carrier is
# being tested.  This is determined by the presense of OOB_BOOT_FIRMWARE and
# OOB_LINUX_IMAGE files on the SD card.  If the files are present, then the 
# SOM tests are run along with programming steps for QSPI and eMMC NVM 
# devices.
CARRIER_PART_NUMBER=AES-US2CAR-1
SOM_PART_NUMBER=AES-US1SOM-1
SOM_ONLY_TESTS=-1

# Information about where test applications are located and what they are named:
PL_GPIO_LOOPBACK_TEST_APP=linux_pl_gpio_loopback_test.elf
PS_PMOD_LOOPBACK_TEST_APP=linux_ps_pmod_loopback_test.elf
USB3_LINK_TEST_APP=linux_usb3_link_test.sh
USER_SWITCH_TEST_APP=linux_user_switch_test.elf
USER_LED_TEST_APP=linux_user_led_test.elf

# Setup variables to store individual test results into.
USB_DEVICE_READ_TEST_RESULT=-1
USB3_LINK_TEST_RESULT=-1
ETHERNET_PING_TEST_RESULT=-1

PL_GPIO_LOOPBACK_TEST_RESULT=-1
PS_PMOD_LOOPBACK_TEST_RESULT=-1
USER_SWITCH_TEST_RESULT=-1
OOB_IMAGE_PROGRAM_RESULT=-1

# Launch the user LED blink test application into the background.
${TEST_APP_FOLDER}/${USER_LED_TEST_APP}&

# Checks to see if OOB boot firmware files are present.  If they are present,
# then the factory test also includes the SOM only portion of tests by setting
# SOM_ONLY_TESTS to 1.
check_for_som_oob_boot_firmware_files ()
{
  if [ -e ${OOB_IMAGE_PRI_FOLDER}/${OOB_BOOT_FIRMWARE} ]
  then
    if [ -f ${OOB_IMAGE_PRI_FOLDER}/${OOB_BOOT_FIRMWARE} ]
    then
      OOB_IMAGE_FOLDER=${OOB_IMAGE_PRI_FOLDER}
      echo "OOB Boot Firmware file found at ${OOB_IMAGE_FOLDER}/${OOB_BOOT_FIRMWARE}"
      SOM_ONLY_TESTS=1
    else
      echo " "
      echo "The OOB Boot Firmware file ${OOB_BOOT_FIRMWARE} does not exist in this folder, ${OOB_IMAGE_FOLDER}"
      echo "WARNING:  Carrier only tests will be performed!"
      echo " "
    fi
  elif [ -e ${OOB_IMAGE_ALT_FOLDER}/${OOB_BOOT_FIRMWARE} ]
  then
    if [ -f ${OOB_IMAGE_ALT_FOLDER}/${OOB_BOOT_FIRMWARE} ]
    then
      OOB_IMAGE_FOLDER=${OOB_IMAGE_ALT_FOLDER}
      echo "OOB Linux Firmware file found at ${OOB_IMAGE_FOLDER}/${OOB_BOOT_FIRMWARE}"
      SOM_ONLY_TESTS=1
    else
      echo " "
      echo "The OOB Boot Firmware file ${OOB_BOOT_FIRMWARE} does not exist in this folder, ${OOB_IMAGE_FOLDER}"
      echo "WARNING:  Carrier only tests will be performed!"
      echo " "
    fi
  else
      echo " "
      echo "Unable to program QSPI with OOB Boot Firmware"
      echo "WARNING:  Carrier only tests will be performed!"
      echo " "
  fi
}

# Checks to see if OOB Linux image files are present and also determine 
# where they are located.
check_for_som_oob_linux_image_files ()
{
  if [ -e ${OOB_IMAGE_FOLDER}/${OOB_LINUX_IMAGE} ]
  then
    echo "OOB Linux Firmware file found at ${OOB_IMAGE_FOLDER}/${OOB_LINUX_IMAGE}"
  else
    echo " "
    echo "The OOB Linux Firmware file ${OOB_LINUX_IMAGE} does not exist in this folder, ${OOB_IMAGE_FOLDER}"
    echo " "
  fi
}


# Displays the Factory Acceptance Test start banner.
print_fat_start_banner ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"                

  # If only SOM tests are being run, display the SOM part number 
  if [ $SOM_ONLY_TESTS == "1" ];
  then
    echo "*** STARTING FACTORY TEST OF ALL ${SOM_PART_NUMBER} PERIPHERALS      ***"
  else
    echo "*** STARTING FACTORY TEST OF ALL ${CARRIER_PART_NUMBER} PERIPHERALS      ***"
  fi

  echo "***                                                            ***"
  echo "******************************************************************"
  echo " "
}

# Performs all of the tests that are common to both SOM and Carrier boards.
launch_all_common_tests ()
{
  # The USB test involves mounting the test USB drive and checking that a 
  # file can be read from the removable drive.
  echo " "
  echo "+++ Running USB Device Read Test..."
  echo " "

  # Check to see if the USB mass storage block device is enumerated.
  if [ -b /dev/sda1 ]
  then
    mount /dev/sda1 /mnt/

    echo "Avnet is one of the world's largest electronics components, technology, distribution and services companies, offering leading design chain services combined with world-class supply chain services in support of the electronics industry. It is an operating group of Avnet, Inc. 

Avnet is focused on exceeding customers'​ expectations. We serve electronic original equipment manufacturers (EOEMs) and electronic manufacturing services (EMS) providers in more than 80 countries, distributing electronic components and embedded solutions from leading manufacturers. From design to delivery, we are a company that is dedicated to support across the board. 

Avnet Americas specifically serves customers in the United States, Canada, Mexico and Brazil. Providing leading products, training and support to the electronics community.

Bring the power of the world's largest distributor to your desktop. Get access to over 5 million products, including pricing and availability, datasheets, cross reference and parametric data and more on the Avnet e-commerce site: http://avnetexpress.avnet.com/

Follow Avnet on Twitter: @AvnetDesignWire

Our Markets and Technologies include:
Automotive
Defense & Aerospace
Embedded Vision
Internet of Things
IP & E
Programmable Logic
& More!

Specialties
Electronics Components Design-Chain, Electronics Components Supply-Chain, Electronics Components Distribution, Embedded Solutions, Interconnect, Passive, Electromechanical Components

Website
http://www.products.avnet.com

Industry
Semiconductors

Type
Public Company

Headquarters
2211 S 47th St Phoenix, AZ 85034 United States

Company Size
10,001+ employees

Founded
1921" > /tmp/usb_device_read_golden.txt

    sync
    cp -f /tmp/usb_device_read_golden.txt /mnt/usb_device_read_test.txt
    sync

    if [ -f /mnt/usb_device_read_test.txt ]
    then
      diff /tmp/usb_device_read_golden.txt /mnt/usb_device_read_test.txt
      USB_DEVICE_READ_TEST_RESULT=$?
    else
      echo "******************************************************************"
      echo " "
      echo "   Test file not found on USB Thumb Drive, make sure that you are"
      echo "   using the USB test thumb drive and re-run factoy tests"
      echo " "
      echo "******************************************************************"
      echo " "
    fi
	
    umount /mnt/
  else
    echo "******************************************************************"
    echo " "
    echo "   No USB Mass Storage Block Device Enumerated!"
    echo "   Connect the test USB Thumb drive to Carrier board"
    echo "   USB Type-A connector and re-run factory tests"
    echo " "
    echo "******************************************************************"
    echo " "
  fi

  if [ $USB_DEVICE_READ_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USB READ TEST:             PASS                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USB READ TEST:             FAIL                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  # Launch the Carrier USB3 LINK test.
  if [ -e ${TEST_APP_FOLDER}/${USB3_LINK_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${USB3_LINK_TEST_APP} ]
    then
      echo " "
      echo "+++ Running USB3 LINK Test..."
      echo " "
      ${TEST_APP_FOLDER}/${USB3_LINK_TEST_APP}
      USB3_LINK_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The File ${USB3_LINK_TEST_APP} Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
    echo " "
  fi

  if [ $USB3_LINK_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USB3 LINK TEST:            PASS                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USB3 LINK TEST:            FAIL                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  # Launch the PL GPIO loopback test.
  if [ -e ${TEST_APP_FOLDER}/${PL_GPIO_LOOPBACK_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${PL_GPIO_LOOPBACK_TEST_APP} ]
    then
      echo " "
      echo "+++ Running PL GPIO Loopback Test..."
      echo " "
      ${TEST_APP_FOLDER}/${PL_GPIO_LOOPBACK_TEST_APP}
      PL_GPIO_LOOPBACK_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The File ${PL_GPIO_LOOPBACK_TEST_APP} Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $PL_GPIO_LOOPBACK_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PL GPIO/PMOD LOOPBACK:     PASS                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PL GPIO/PMOD LOOPBACK:     FAIL                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  echo " "
  echo "+++ Running Ethernet Ping Test..."
  echo " "
  ping -c 10 192.168.1.10
  ETHERNET_PING_TEST_RESULT=$?

  if [ $ETHERNET_PING_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET PING TEST:        PASS                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** ETHERNET PING TEST:        FAIL                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}
}

# Performs all of the tests that are related to the SOM only.
launch_all_som_specific_tests ()
{
  # Launch the PS Pmod loopback test.
  if [ -e ${TEST_APP_FOLDER}/${PS_PMOD_LOOPBACK_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${PS_PMOD_LOOPBACK_TEST_APP} ]
    then
      echo " "
      echo "+++ Running PS Pmod Loopback Test..."
      echo " "
      ${TEST_APP_FOLDER}/${PS_PMOD_LOOPBACK_TEST_APP}
      PS_PMOD_LOOPBACK_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The File ${PS_PMOD_LOOPBACK_TEST_APP} Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $PS_PMOD_LOOPBACK_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PS PMOD LOOPBACK:          PASS                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** PS PMOD LOOPBACK:          FAIL                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  # Launch the User Switch test.
  if [ -e ${TEST_APP_FOLDER}/${USER_SWITCH_TEST_APP} ]
  then
    if [ -f ${TEST_APP_FOLDER}/${USER_SWITCH_TEST_APP} ]
    then
      echo " "
      echo "+++ Running User Switch Test..."
      echo " "
      ${TEST_APP_FOLDER}/${USER_SWITCH_TEST_APP}
      USER_SWITCH_TEST_RESULT=$?
    fi
  else
    echo " "
    echo "The File ${USER_SWITCH_TEST_APP} Does Not Exist at this Directory, ${TEST_APP_FOLDER}"
    echo " "
  fi

  sleep ${SLEEP_INTERVAL_NORMAL}

  if [ $USER_SWITCH_TEST_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USER SWITCH:               PASS                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** USER SWITCH:               FAIL                            ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi
}

# Programs the OOB boot images to QSPI.  Used on SOM FAT runs.
program_oob_boot_firmware_to_qspi ()
{
  if [ -f ${OOB_IMAGE_FOLDER}/${OOB_BOOT_FIRMWARE} ]
  then		
    echo " "
    echo "+++ Programming QSPI OOB Image..."
    echo " "
    flashcp ${OOB_IMAGE_FOLDER}/${OOB_BOOT_FIRMWARE} /dev/mtd0
    OOB_IMAGE_PROGRAM_RESULT=$?
  fi
}

# Programs the OOB Linux images to eMMC.  Used on SOM FAT runs.
program_oob_linux_image_to_emmc ()
{
  echo " "
  echo "+++ Erasing eMMC Partition Table..."
  echo " "
  umount /run/media/mmcblk0*
  dd if=/dev/zero of=/dev/mmcblk0 bs=1M count=100
  sync

  echo " "
  echo "+++ Creating eMMC Partition Table..."
  echo " "
# Feed commands into fdisk to create an initial partition table.
echo "n
p
1
1
+256M
v
w" | fdisk /dev/mmcblk0 && sleep 3 && sync

# Then use fdisk to wipe the partition table out.
echo "d
w" | fdisk /dev/mmcblk0 && sleep 3 && sync

# fdisk - third time is the charm!
echo "n
p
1
1
+256M
v
w" | fdisk /dev/mmcblk0 && sleep 3 && sync

  sync

  echo " "
  echo "+++ Formatting eMMC Boot Partition to FAT32..."
  echo " "
  mkdosfs -F 32 /dev/mmcblk0p1
  sync

  echo " "
  echo "+++ Loading eMMC OOB Image..."
  echo " "
			
  mount /dev/mmcblk0p1 /mnt/
  cp ${OOB_IMAGE_FOLDER}/${OOB_LINUX_IMAGE} /mnt/image.ub
  OOB_IMAGE_PROGRAM_RESULT=$?
  sync

  # Unmount the destination eMMC partition.			
  umount /mnt/

  # Show some more meaningful results to the test operator.
  sleep ${SLEEP_INTERVAL_NORMAL}
  if [ $OOB_IMAGE_PROGRAM_RESULT == "0" ]; 
  then
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** OOB IMAGE PROGRAMMING:       PASS                          ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  else
    echo " "
    echo "******************************************************************"
    echo "***                                                            ***"                
    echo "*** OOB IMAGE PROGRAMMING:       FAIL                          ***"
    echo "***                                                            ***"
    echo "******************************************************************"
    echo " "
  fi
}

# Displays the banner header for the final results.
print_fat_results_banner_header ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"

  if [ $SOM_ONLY_TESTS == "1" ];
  then
    echo "***   ${SOM_PART_NUMBER} Factory Test Build V${FAT_VERSION_NUMBER} Complete            ***"
  else
    echo "***   ${CARRIER_PART_NUMBER} Factory Build Test V${FAT_VERSION_NUMBER} Complete            ***"
  fi

  echo "***                                                            ***"
  echo "******************************************************************"
}

# Displays also the test results for all common tests.
print_fat_results_common_tests ()
{
  echo "***                                                            ***"

  if [ $USB_DEVICE_READ_TEST_RESULT == "0" ]; 
  then
    echo "*** USB Read Link Test:         PASS                           ***"
  else
    echo "*** USB Read Link Test:     --- FAIL ---                       ***"
  fi

  if [ $USB3_LINK_TEST_RESULT == "0" ]; 
  then
    echo "*** USB3 Link Test:             PASS                           ***"
  else
    echo "*** USB3 Link Test:         --- FAIL ---                       ***"
  fi

  if [ $PL_GPIO_LOOPBACK_TEST_RESULT == "0" ]; 
  then
    echo "*** PL GPIO Loopback Test:      PASS                           ***"
  else
    echo "*** PL GPIO Loopback Test:  --- FAIL ---                       ***"
  fi

  if [ $ETHERNET_PING_TEST_RESULT == 0 ];
  then
    echo "*** Ethernet Ping:              PASS                           ***"
  else
    echo "*** Ethernet Ping:          --- FAIL ---                       ***"
  fi
}

# Displays also the test results for the SOM.
print_fat_results_som_specific_tests ()
{
  echo "***                                                            ***"

  if [ $PS_PMOD_LOOPBACK_TEST_RESULT == "0" ];
  then
    echo "*** PS Pmod Loopback Test:      PASS                           ***"
  else
    echo "*** PS Pmod Loopback Test:  --- FAIL ---                       ***"
  fi

  if [ $USER_SWITCH_TEST_RESULT == "0" ];
  then
    echo "*** User Switch Test:           PASS                           ***"
  else
    echo "*** User Switch Test:       --- FAIL ---                       ***"
  fi

  if [ $OOB_IMAGE_PROGRAM_RESULT == "0" ]; 		
  then
    echo "*** OOB Image Programming:      PASS                           ***"
  else
    echo "*** OOB Image Programming:  --- FAIL ---                       ***"
  fi

  echo "***                                                            ***"
}

# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# First, discover if the OOB boot firmware files exist in either the primary
# or alternate expected locations.  If those files do not exist in either 
# folder, then this is a FAT run for carrier card only because SOM_ONLY_TESTS 
# is expected to be set to 1.
check_for_som_oob_boot_firmware_files

# If firmware was discovered for booting, also look for Linux image files 
# which need to be programmed also.
if [ $SOM_ONLY_TESTS == "1" ];
then
  check_for_som_oob_linux_image_files
fi

# Display the "Start" test banner so that the test operator can see that the 
# FAT is starting.
print_fat_start_banner

# Run all of the tests that are common to both SOM and Carrier boards here.
launch_all_common_tests

# If this is a SOM FAT run, then launch all of the SOM specific tests here.
if [ $SOM_ONLY_TESTS == "1" ];
then
  launch_all_som_specific_tests
fi 

# If this is a SOM FAT run, program the OOB firmware images to QSPI.
if [ $SOM_ONLY_TESTS == "1" ];
then
  program_oob_boot_firmware_to_qspi
fi

# If this is a SOM FAT run, program the OOB Linux image(s) to eMMC.
if [ $SOM_ONLY_TESTS == "1" ];
then
  # But only if the QSPI programming was successful.
  if [ $OOB_IMAGE_PROGRAM_RESULT == "0" ];
  then
    program_oob_linux_image_to_emmc
  fi
fi

# Display the "Results" test banner so that the test operator can see that 
# the FAT is complete.
print_fat_results_banner_header

# Show test results for tests that are common among the SOM and Carrier
# FAT procedures.
print_fat_results_common_tests

# If this is a SOM FAT run, show test results for tests that are only for
# the SOM FAT procedure.
if [ $SOM_ONLY_TESTS == "1" ];
then
  print_fat_results_som_specific_tests
fi

echo "******************************************************************"
echo " "


# END HERE: All done with the testing.
