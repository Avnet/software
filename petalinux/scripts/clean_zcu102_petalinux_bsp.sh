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
#  Create Date:         Jul 27, 2016
#  Design Name:         Xilinx ZCU102 PetaLinux BSP Generator
#  Module Name:         clean_zcu102_petalinux_bsp.tcl
#  Project Name:        Xilinx ZCU102 PetaLinux BSP Generator
#  Target Devices:      Xilinx Zynq Ultrascale MPSoC
#  Hardware Boards:     Xilinx ZCU102
# 
#  Tool versions:       Xilinx Vivado 2016.2
# 
#  Description:         Clean Script for ZCU102 PetaLinux BSP HW Platform
# 
#  Dependencies:        None
#
#  Revision:            Jul 27, 2016: 1.00 Initial version
# 
# ----------------------------------------------------------------------------

#!/bin/bash

# Set global variables here.
HDL_HARDWARE_NAME=zcu102_petalinux_hw
HDL_PROJECT_NAME=zcu102_petalinux
HDL_PROJECTS_FOLDER=../../../hdl/Projects
HDL_SCRIPTS_FOLDER=../../../hdl/Scripts
PETALINUX_APPS_FOLDER=../../../software/petalinux/apps
PETALINUX_PROJECTS_FOLDER=../../../software/petalinux/projects
PETALINUX_SCRIPTS_FOLDER=../../../software/petalinux/scripts
START_FOLDER=`pwd`

clean_petalinux_bsp ()
{ 
  # Remove the hardware platform project.
  rm -rf ${START_FOLDER}/${HDL_PROJECTS_FOLDER}/${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}

  # Remove the PetaLinux project.
  rm -rf ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Remove the PetaLinux BSP output.
  rm -rf ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}.bsp

  # Change to PetaLinux scripts folder
  cd ${START_FOLDER}/${PETALINUX_SCRIPTS_FOLDER}
}

main_clean_function ()
{
  # This function is responsible for cleaning all of the hardware
  # platforms followed by removal of all PetaLinux project associated with
  # the targets.

  #
  # Remove project files created for PetaLinux BSP on the ZCU102 target.
  #
  HDL_BOARD_NAME=ZCU102
  PETALINUX_PROJECT_NAME=zcu102_2016_2
  clean_petalinux_bsp

}

# Call the main_clean_function declared above to start cleaning up all 
# output products.
main_clean_function
