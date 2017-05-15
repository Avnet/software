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
#  Module Name:         setup_eth1.sh
#  Project Name:        UltraZed I/O Carrier ETH1 Setup Script
#  Target Devices:      Xilinx Zynq UltraScale MPSoC
#  Hardware Boards:     UltraZed + I/O Carrier
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         UltraZed I/O Carrier ETH1 Setup Script
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

setup_eth1()
{
	# Edit the /etc/network/interfaces file to add the ethernet interface for the 
	# WNC M14A2A shield	
	echo "Add eth1 interface to /etc/network/interfaces..."
	echo "#" >> /etc/network/interfaces
	echo "auto eth1" >> /etc/network/interfaces
	echo "iface eth1 inet dhcp" >> /etc/network/interfaces
	sleep ${SLEEP_INTERVAL_NORMAL}
	echo "ifconfig eth1 up"
	ifconfig eth1 up
	sleep ${SLEEP_INTERVAL_LONG}
	echo "ifup eth1"
	ifup eth1
	sleep ${SLEEP_INTERVAL_LONG}
}


# START HERE: Non-boilerplate code above should be contained within 
# functions so that at this point simple high level calls can be made to 
# the bigger blocks above.

# Add and start the ETH1 interface for the cellular modem shield
setup_eth1

# END HERE: All done with eth1 setup
