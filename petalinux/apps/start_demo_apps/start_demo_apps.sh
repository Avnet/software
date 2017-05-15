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
#  Create Date:         March 3, 2017
#  Design Name:         UltraZed + I/O Carrier
#  Module Name:         start_demo_apps.sh
#  Project Name:        UltraZed I/O Carrier Start Demo Apps Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + I/O Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         UltraZed I/O Carrier Start Demo Apps Script
# 
#  Dependencies:        
#
#  Revision:            March 3, 2017: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# script version information.
DEMO_VERSION_NUMBER=1.0

# Information used to program Out Of Box components.
DEMO_APP_FOLDER=/home/root
I2C_TEMP=UltraZed_LTE_IoT_Temp
I2C_HUMIDITY=UltraZed_LTE_IoT_Hum
SPI_THERMOCOUPLE=UltraZed_LTE_IoT_Therm


# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# Start the demo apps
set -m
/home/root/max44000_sensor &
/home/root/IIoT_Quickstart_Demo ${I2C_TEMP} &
/home/root/IIoT_Quickstart_Demo ${I2C_HUMIDITY} 1 &
/home/root/IIoT_Quickstart_Demo ${SPI_THERMOCOUPLE} 3 &
exit 0

# END HERE: All done with demo setup and start
