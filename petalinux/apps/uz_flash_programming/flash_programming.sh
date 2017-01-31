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
#                      Copyright(c) 2017 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------
# 
#  Create Date:         Jan 12, 2017
#  Design Name:         UltraZed-EG SOM
#  Module Name:         flash_programming.sh
#  Project Name:        UltraZed Flash Programming Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + I/O Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         Startup Script for UltraZed In System Flash 
#                       Programming
# 
#  Dependencies:        
#
#  Revision:            Jan 12, 2017: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# Sleep interval between programming operations.
SLEEP_INTERVAL_NORMAL=1

# Information used to program Out Of Box components.
CUSTOM_IMAGE_FOLDER=/run/media/mmcblk1
CUSTOM_IMAGE_PRI_FOLDER=/run/media/mmcblk1
CUSTOM_IMAGE_ALT_FOLDER=/run/media/mmcblk1p1
CUSTOM_BOOT_IMAGE=BOOT_EMMC_CUSTOM.bin
CUSTOM_LINUX_IMAGE=image.CUSTOM.ub

# Variables used to determine whether Flash programming will be carried out 
# or not.  This is determined by the presense of CUSTOM_BOOT_IMAGE and
# CUSTOM_LINUX_IMAGE files on the SD card.  If the files are present, then  
# the SOM Flash QSPI and eMMC NVM devices are programmed.
CUSTOM_BOOT_IMAGE_IDENTIFIED=-1
CUSTOM_LINUX_IMAGE_IDENTIFIED=-1
CUSTOM_BOOT_IMAGE_PROGRAM_RESULT=-1
CUSTOM_LINUX_IMAGE_PROGRAM_RESULT=-1

# Checks to see if CUSTOM boot image files are present.  If they are 
# present, then the files are programmed into the respective flash devices.
check_for_custom_boot_image_files ()
{
  if [ -e ${CUSTOM_IMAGE_PRI_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
  then
    if [ -f ${CUSTOM_IMAGE_PRI_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
    then
      CUSTOM_IMAGE_FOLDER=${CUSTOM_IMAGE_PRI_FOLDER}
      echo "CUSTOM Boot Image file found at ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE}"
      CUSTOM_BOOT_IMAGE_IDENTIFIED=1
    else
      echo " "
      echo "The CUSTOM Boot Image file ${CUSTOM_BOOT_IMAGE} does not exist in this folder, ${CUSTOM_IMAGE_FOLDER}"
      echo " "
    fi
  elif [ -e ${CUSTOM_IMAGE_ALT_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
  then
    if [ -f ${CUSTOM_IMAGE_ALT_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
    then
      CUSTOM_IMAGE_FOLDER=${CUSTOM_IMAGE_ALT_FOLDER}
      echo "CUSTOM Boot Image file found at ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE}"
      CUSTOM_BOOT_IMAGE_IDENTIFIED=1
    else
      echo " "
      echo "The CUSTOM Boot Image file ${CUSTOM_BOOT_IMAGE} does not exist in this folder, ${CUSTOM_IMAGE_FOLDER}"
      echo " "
    fi
  else
      echo " "
      echo "Unable to program QSPI with CUSTOM Boot Image"
      echo " "
  fi
}

# Checks to see if custom Linux image files are present and also determine 
# where they are located.
check_for_custom_linux_image_files ()
{
  if [ -e ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_LINUX_IMAGE} ]
  then
    echo "CUSTOM Linux Image file found at ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_LINUX_IMAGE}"
    CUSTOM_LINUX_IMAGE_IDENTIFIED=1
  else
    echo " "
    echo "The CUSTOM Linux Image file ${CUSTOM_LINUX_IMAGE} does not exist in this folder, ${CUSTOM_IMAGE_FOLDER}"
    echo " "
  fi
}


# Programs the custom boot images to QSPI and captures the results.
program_custom_boot_image_to_qspi ()
{
  if [ -f ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE} ]
  then		
    echo " "
    echo "+++ Programming QSPI CUSTOM Boot Image..."
    echo " "
    flashcp ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_BOOT_IMAGE} /dev/mtd0
    CUSTOM_BOOT_IMAGE_PROGRAM_RESULT=$?
  fi
}

# Programs the custom Linux images to eMMC and captures the results.
program_custom_linux_image_to_emmc ()
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
  echo "+++ Loading eMMC CUSTOM Image..."
  echo " "
			
  mount /dev/mmcblk0p1 /mnt/
  cp ${CUSTOM_IMAGE_FOLDER}/${CUSTOM_LINUX_IMAGE} /mnt/image.ub
  CUSTOM_LINUX_IMAGE_PROGRAM_RESULT=$?
  sync

  # Unmount the destination eMMC partition.			
  umount /mnt/
}

# Displays the banner header for the programming results.
print_programming_results_banner_header ()
{
  echo " "
  echo "******************************************************************"
  echo "***                                                            ***"
  echo "***   UltraZed-EG Custom Flash Image Programming               ***"
  echo "***                                                            ***"
  echo "******************************************************************"
}

# Displays also the individual programming results.
print_programming_results ()
{
  echo "***                                                            ***"

  if [ $CUSTOM_BOOT_IMAGE_PROGRAM_RESULT == "0" ]; 		
  then
    echo "*** Custom Boot Image QSPI Flash Programming:    SUCCESS       ***"
  else
    echo "*** Custom Boot Image QSPI Flash Programming:  --- FAIL ---    ***"
  fi

  if [ $CUSTOM_LINUX_IMAGE_PROGRAM_RESULT == "0" ]; 		
  then
    echo "*** Custom Linux Image eMMC Flash Programming:   SUCCESS       ***"
  else
    echo "*** Custom Linux Image eMMC Flash Programming: --- FAIL ---    ***"
  fi

  echo "***                                                            ***"
}

# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# First, discover if the CUSTOM boot image files exist in either the primary
# or alternate expected locations.  If those files do not exist in either 
# folder, then this is a FAT run for carrier card only because SOM_ONLY_TESTS 
# is expected to be set to 1.
check_for_custom_boot_image_files

# If an image was discovered for booting, also look for Linux image files 
# which need to be programmed also.
if [ $CUSTOM_BOOT_IMAGE_IDENTIFIED == "1" ];
then
  check_for_custom_linux_image_files
fi

# Program the CUSTOM boot images to QSPI.
if [ $CUSTOM_BOOT_IMAGE_IDENTIFIED == "1" ];
then
  program_custom_boot_image_to_qspi
fi

# Program the CUSTOM Linux image(s) to eMMC.
if [ $CUSTOM_LINUX_IMAGE_IDENTIFIED == "1" ];
then
  # But only if the QSPI programming was already successful.
  if [ $CUSTOM_BOOT_IMAGE_PROGRAM_RESULT == "0" ];
  then
    program_custom_linux_image_to_emmc
  fi
fi

# Display the "Results" test banner so that the test operator can see that 
# the programming.
print_programming_results_banner_header

# Show programming results for flash programming.
print_programming_results

echo "******************************************************************"
echo " "


# END HERE: All done with the programming.
