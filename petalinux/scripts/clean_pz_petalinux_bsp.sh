#!/bin/bash

# Set global variables here.
HDL_HARDWARE_NAME=pz_petalinux_hw
HDL_PROJECT_NAME=pz_petalinux
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
  # Remove project files created for PetaLinux BSP on the PZ7010_FMC2 target.
  #
  HDL_BOARD_NAME=PZ7010_FMC2
  PETALINUX_PROJECT_NAME=pz_7010_2015_2_1
  clean_petalinux_bsp

  #
  # Remove project files created for PetaLinux BSP on the PZ7015_FMC2 target.
  #
  HDL_BOARD_NAME=PZ7015_FMC2
  PETALINUX_PROJECT_NAME=pz_7015_2015_2_1
  clean_petalinux_bsp

  #
  # Remove project files created for PetaLinux BSP on the PZ7020_FMC2 target.
  #
  HDL_BOARD_NAME=PZ7020_FMC2
  PETALINUX_PROJECT_NAME=pz_7020_2015_2_1
  clean_petalinux_bsp

  #
  # Remove project files created for PetaLinux BSP on the PZ7030_FMC2 target.
  #
  HDL_BOARD_NAME=PZ7030_FMC2
  PETALINUX_PROJECT_NAME=pz_7030_2015_2_1
  clean_petalinux_bsp
}

# Call the main_clean_function declared above to start cleaning up all 
# output products.
main_clean_function
