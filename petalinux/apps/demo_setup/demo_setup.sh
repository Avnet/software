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
#  Module Name:         demo_setup.sh
#  Project Name:        UltraZed I/O Carrier Demo Setup Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + I/O Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         UltraZed I/O Carrier Demo Setup Script
# 
#  Dependencies:        
#
#  Revision:            March 3, 2017: 1.0 Initial version
# 
# ----------------------------------------------------------------------------
#!/etc/sh

# script version information.
DEMO_VERSION_NUMBER=1.0

# Sleep interval between tests.
SLEEP_INTERVAL_NORMAL=2
SLEEP_INTERVAL_LONG=10

# Variables for location and names of kernel modules to be loaded
MODULES_FOLDER=/lib/modules/4.4.0/extra
TEMP_SENSOR_KO=hts221.ko
PRS_SENSOR_KO=lps25.ko
MAG_SENSOR_KO=lis3mdl_mag.ko
PRX_SENSOR_KO=max44000.ko

insert_kernel_modules()
{
# Insert the kernel modules for the sensor devices on tghe
	insmod ${MODULES_FOLDER}/${TEMP_SENSOR_KO}
	sleep ${SLEEP_INTERVAL_NORMAL}
	insmod ${MODULES_FOLDER}/${PRS_SENSOR_KO}
	sleep ${SLEEP_INTERVAL_NORMAL}
	insmod ${MODULES_FOLDER}/${MAG_SENSOR_KO}
	sleep ${SLEEP_INTERVAL_NORMAL}
	insmod ${MODULES_FOLDER}/${PRX_SENSOR_KO}
	sleep ${SLEEP_INTERVAL_NORMAL}
}


# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# First, load the kernel modules
insert_kernel_modules

# END HERE: All done with loading kernel modules
