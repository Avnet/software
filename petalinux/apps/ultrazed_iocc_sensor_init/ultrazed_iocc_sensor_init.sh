#!/bin/sh

# Show the application banner.
echo " "
echo "*********************************************************************"
echo "***                                                               ***"
echo "***   Avnet UltraZed + I/O Carrier Card Sensors Design            ***"
echo "***                                                               ***"
echo "***   Script to load the kernel modules for sensors at boot       ***"
echo "***                                                               ***"
echo "***   Based Upon the UZ3EG 2016.2 PetaLinux BSP from Avnet        ***"
echo "***                                                               ***"
echo "***   More information available on UltraZed.org community site   ***"
echo "***                                                               ***"
echo "*********************************************************************"
echo " "

# load the kernel modules for sensors on the ST Micro X-NUCLEO-IKS01A1 sensor shield.
insmod /lib/modules/4.4.0/extra/hts221.ko
insmod /lib/modules/4.4.0/extra/lis3mdl.ko
insmod /lib/modules/4.4.0/extra/lps25.ko



