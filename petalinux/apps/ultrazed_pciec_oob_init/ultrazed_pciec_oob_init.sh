#!/bin/sh

# Show the application banner.
echo " "
echo "*********************************************************************"
echo "***                                                               ***"
echo "***   Avnet UltraZed Out Of Box PetaLinux Build V1.0              ***"
echo "***                                                               ***"
echo "***   Based Upon the UZ3EG 2016.2 PetaLinux BSP from Avnet        ***"
echo "***                                                               ***"
echo "***   More information available on UltraZed.org community site   ***"
echo "***                                                               ***"
echo "*********************************************************************"
echo " "

# Start blinking the PS user LED.
/home/root/linux_ps_led_blink.elf --interval 2 --mio 26 &


