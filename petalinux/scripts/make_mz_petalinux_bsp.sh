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
#  Create Date:         Mar 26, 2016
#  Design Name:         MicroZed PetaLinux BSP Generator
#  Module Name:         make_mz_petalinux_bsp.tcl
#  Project Name:        MicroZed PetaLinux BSP Generator
#  Target Devices:      Xilinx Zynq-7000
#  Hardware Boards:     MicroZed SOM
# 
#  Tool versions:       Xilinx Vivado 2015.2
# 
#  Description:         Build Script for MicroZed PetaLinux BSP HW Platform
# 
#  Dependencies:        None
#
#  Revision:            Mar 26, 2016: 1.00 Initial version
# 
# ----------------------------------------------------------------------------

#!/bin/bash

# Set global variables here.
BUILD_BOOT_QSPI_OPTION=no
BUILD_BOOT_EMMC_OPTION=no
BUILD_BOOT_EMMC_NO_BIT_OPTION=no
FSBL_PROJECT_NAME=zynq_fsbl_app
HDL_HARDWARE_NAME=mz_petalinux_hw
HDL_PROJECT_NAME=mz_petalinux
HDL_PROJECTS_FOLDER=../../../hdl/Projects
HDL_SCRIPTS_FOLDER=../../../hdl/Scripts
PETALINUX_APPS_FOLDER=../../../software/petalinux/apps
PETALINUX_CONFIGS_FOLDER=../../../software/petalinux/configs
PETALINUX_PROJECTS_FOLDER=../../../software/petalinux/projects
PETALINUX_SCRIPTS_FOLDER=../../../software/petalinux/scripts
START_FOLDER=`pwd`

source_tools_settings ()
{
  # Source the tools settings scripts so that both Vivado and PetaLinux can 
  # be used throughout this build script.
  source /opt/Xilinx/Vivado/2015.2/settings64.sh
  source /opt/petalinux-v2015.2.1-final/settings.sh
}

petalinux_project_restore_boot_config ()
{
  # Restore original PetaLinux project config. Don't forget that the
  # petalinux_project_save_boot_config () should have been called at some
  # point before this function gets called, otherwise there probably is
  # nothing there to restore.
  echo " "
  echo "Restoring original PetaLinux project config ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/
  cp config.orig config
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Restore original U-Boot top level configuration.
  echo " "
  echo "Restoring original U-Boot top level configuration..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  cp platform-top.h.orig platform-top.h
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Restore original U-Boot environment configuration.
  echo " "
  echo "Restoring original U-Boot environment configuration ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  cp platform-auto.h.orig platform-auto.h
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}
}

petalinux_project_save_boot_config ()
{
  # Save original PetaLinux project config.
  echo " "
  echo "Saving original PetaLinux project config ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/
  cp config config.orig
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Save original U-Boot top level configuration.
  echo " "
  echo "Saving original U-Boot top level configuration..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  cp platform-top.h platform-top.h.orig
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Save original U-Boot environment configuration.
  echo " "
  echo "Saving original U-Boot environment configuration ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  cp platform-auto.h platform-auto.h.orig
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}
}

petalinux_project_set_boot_config_emmc ()
{ 
  # Change PetaLinux project config to boot from eMMC (via QSPI).
  echo " "
  echo "Patching project config for eMMC boot support..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/config.emmc_boot.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Add support for QSPI + eMMC boot to U-Boot environment configuration.
  echo " "
  echo "Applying patch to add QSPI + eMMC boot support in U-Boot ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/u-boot/platform-auto.h.emmc_boot.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}
}

petalinux_project_set_boot_config_emmc_no_bit ()
{ 
  # Change PetaLinux project config to boot from eMMC (via QSPI).
  echo " "
  echo "Patching project config for eMMC boot support..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/config.emmc_boot.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Add support for eMMC commands to U-Boot top level configuration which
  # allows for bistream to be loaded from eMMC instead of BOOT.BIN in QSPI
  # flash.
  echo " "
  echo "Applying patch to add eMMC bitstream load support in U-Boot ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/u-boot/platform-top.h.emmc_boot_no_bit.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Add support for QSPI + eMMC boot to U-Boot environment configuration.
  echo " "
  echo "Applying patch to add QSPI + eMMC boot support in U-Boot ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/u-boot/platform-auto.h.emmc_boot.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}
}

petalinux_project_set_boot_config_qspi ()
{ 
  # Change PetaLinux project config to boot from QSPI.
  echo " "
  echo "Patching project config for QSPI boot support..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/config.qspi_boot.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Add support for QSPI commands to U-Boot top level configuration.
  echo " "
  echo "Applying patch to add QSPI command support in U-Boot ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/u-boot/platform-top.h.add_qspi_support.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Add support for QSPI boot to U-Boot environment configuration.
  echo " "
  echo "Applying patch to add QSPI boot support in U-Boot ..."
  echo " "
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/u-boot
  patch < ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/u-boot/platform-auto.h.qspi_boot.patch
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}
}

create_petalinux_bsp ()
{ 
  # This function is responsible for creating a PetaLinux BSP around the
  # hardware platform specificed in HDL_PROJECT_NAME variable and build
  # the PetaLinux project within the folder specified by the 
  # PETALINUX_PROJECT_NAME variable.
  #
  # When complete, the BSP should boot from SD card 

  # Change to PetaLinux projects folder.
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}

  # Create the PetaLinux project.
  petalinux-create --type project --template zynq --name ${PETALINUX_PROJECT_NAME}

  # Create the hardware definition folder.
  mkdir -p ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform

  # Import the hardware definition files and hardware platform bitstream from
  # implemented system products folder.
  cd ${START_FOLDER}/${HDL_PROJECTS_FOLDER}

  echo " "
  echo "Importing hardware definition ${HDL_HARDWARE_NAME} from impl_1 folder ..."
  echo " "

  cp -f ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}/${HDL_PROJECT_NAME}.runs/impl_1/${HDL_PROJECT_NAME}_wrapper.sysdef \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/${HDL_HARDWARE_NAME}.hdf

  echo " "
  echo "Importing hardware bitstream ${HDL_HARDWARE_NAME} from impl_1 folder ..."
  echo " "

  cp -f ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}/${HDL_PROJECT_NAME}.runs/impl_1/${HDL_PROJECT_NAME}_wrapper.bit \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/${HDL_HARDWARE_NAME}.bit

  # Import the First Stage Boot Loader application executable from the SDK
  # workspace FSBL application folder.

  echo " "
  echo "Importing FSBL from ${HDL_HARDWARE_NAME}.sdk workspace ..."
  echo " "

  cp -f ${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}/${HDL_PROJECT_NAME}.sdk/${FSBL_PROJECT_NAME}/Release/${FSBL_PROJECT_NAME}.elf \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/hw_platform/${FSBL_PROJECT_NAME}.elf

  # Change directories to the hardware definition folder for the PetaLinux
  # project, at this point the .hdf file must be located in this folder 
  # for the petalinux-config step to be successful.
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}

  # Import the hardware description into the PetaLinux project.
  petalinux-config --oldconfig --get-hw-description=./hw_platform/ -p ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}
 
  # Overwrite the PetaLinux project config with the revision controlled source
  # config.  Use a board specific configuration if available, if not, use the 
  # generic one by default.
  echo " "
  echo "Overwriting PetaLinux project config ..."
  echo " "
  if [ -f ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/config.${HDL_BOARD_NAME} ] 
    then
    cp -rf ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/config.${HDL_BOARD_NAME} \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/config
  else
    cp -rf ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/config.generic \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/config
  fi

  # Create a PetaLinux application named weaved.
  petalinux-create --type apps --name weaved --enable

  # Copy the weaved application information over to the weaved 
  # application folder.
  cp -rf ${START_FOLDER}/${PETALINUX_APPS_FOLDER}/weaved/* \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/components/apps/weaved

  # Copy the weaved application binary over to the the weaved application
  # folder.
  cp -f ~/demo/weaved/weavedOEM/weavedConnectd.arm7l \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/components/apps/weaved/weavedOEM/

  # Create a PetaLinux application named force_usb_power.
  petalinux-create --type apps --name force_usb_power --enable

  # Copy the force_usb_power application information over to the force_usb_power 
  # application folder.
  cp -rf ${START_FOLDER}/${PETALINUX_APPS_FOLDER}/force_usb_power/* \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/components/apps/force_usb_power

  # Create a PetaLinux application named uWeb_custom.
  petalinux-create --type apps --name uWeb_custom --enable

  # Copy the uWeb_custom application information over to the uWeb_custom 
  # application folder.
  cp -rf ${START_FOLDER}/${PETALINUX_APPS_FOLDER}/uWeb_custom/* \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/components/apps/uWeb_custom

  # Overwrite the defult web page graphic for target board with the image 
  # being targeted, if a replacement is available.
  if [ -f ~/demo/img/${HDL_BOARD_NAME}.png ] 
  then
    echo " "
    echo "Overwriting board photo within uWeb_custom app data ..."
    echo " "
    cp -rf ~/demo/img/${HDL_BOARD_NAME}.png \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/components/apps/uWeb_custom/sp605-2.png
  fi

  # Overwrite the rootfs component config with the revision controlled source
  # config.
  echo " "
  echo "Overwriting rootfs config ..."
  echo " "
  cp -rf ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/rootfs/config \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/rootfs/

  # Overwrite the top level devicetree source with the revision controlled
  # source file.
  echo " "
  echo "Overwriting top level devicetree source ..."
  echo " "
  cp -rf ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/device-tree/system-top.dts \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/device-tree/

  # Overwrite the config level devicetree source with the revision controlled
  # source file.
  echo " "
  echo "Overwriting config level devicetree source ..."
  echo " "
  cp -rf ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/device-tree/system-conf.dtsi \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/subsystems/linux/configs/device-tree/

  # Prepare to modify project configurations.
  petalinux_project_save_boot_config

  # If the QSPI boot option is set, then perform the steps needed to build 
  # BOOT.BIN for booting from QSPI.
  if [ "$BUILD_BOOT_QSPI_OPTION" == "yes" ]
  then
    # Modify the project configuration for QSPI boot.
    petalinux_project_set_boot_config_qspi

    # Make sure that intermediary files get cleaned up.
    petalinux-build -x distclean

    # Build PetaLinux project.
    petalinux-build 

    # Create boot image.
    petalinux-package --boot --fsbl hw_platform/${FSBL_PROJECT_NAME}.elf --fpga hw_platform/${HDL_HARDWARE_NAME}.bit --uboot --force

    # Copy the boot.bin file and name the new file BOOT_QSPI.bin
    cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT.BIN \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_QSPI.bin
  fi

  # Restore project configurations and wipe out any changes made for special 
  # boot options.
  petalinux_project_restore_boot_config

  # If the EMMC boot option is set, then perform the steps needed to build 
  # BOOT.BIN for booting from QSPI + eMMC.
  if [ "$BUILD_BOOT_EMMC_OPTION" == "yes" ]
  then
    # Modify the project configuration for EMMC boot.
    petalinux_project_set_boot_config_emmc

    # Make sure that intermediary files get cleaned up.
    petalinux-build -x distclean

    # Build PetaLinux project.
    petalinux-build 

    # Create boot image.
    petalinux-package --boot --fsbl hw_platform/${FSBL_PROJECT_NAME}.elf --fpga hw_platform/${HDL_HARDWARE_NAME}.bit --uboot --force

    # Copy the boot.bin file and name the new file BOOT_EMMC.bin
    cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT.BIN \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_EMMC.bin
  fi

  # Restore project configurations and wipe out any changes made for special 
  # boot options.
  petalinux_project_restore_boot_config

  # If the EMMC boot option is set, then perform the steps needed to build 
  # BOOT.BIN for booting from QSPI + eMMC with the bistream loaded from eMMC
  # instead of from BOOT.BIN image in QSPI.
  if [ "$BUILD_BOOT_EMMC_NO_BIT_OPTION" == "yes" ]
  then
    # Modify the project configuration for EMMC boot.
    petalinux_project_set_boot_config_emmc_no_bit

    # Make sure that intermediary files get cleaned up.
    petalinux-build -x distclean

    # Build PetaLinux project.
    petalinux-build 

    # Create boot imagewhich does not contain the bistream image.
    petalinux-package --boot --fsbl hw_platform/${FSBL_PROJECT_NAME}.elf --uboot --force

    # Copy the boot.bin file and name the new file BOOT_EMMC_No_Bit.BIN
    cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT.BIN \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_EMMC_No_Bit.BIN

    # Create a temporary Vivado TCL script which take the standard bitstream 
    # file format and modify it to allow u-boot to load it into the 
    # programmable logic on the Zynq device via PCAP interface.
    echo "write_cfgmem -format bin -interface spix1 -loadbit \"up 0x0 hw_platform/${HDL_HARDWARE_NAME}.bit\" -force images/linux/system.bit.bin" > swap_bits.tcl
    
    # Launch vivado in batch mode to clean output products from the hardware platform.
    vivado -mode batch -source swap_bits.tcl

    # Remove the temporary Vivado script.
    rm -f swap_bits.tcl
  fi

  # Restore project configurations and wipe out any changes made for special 
  # boot options.
  petalinux_project_restore_boot_config

  # Make sure that intermediary files get cleaned up.
  petalinux-build -x distclean

  # Build PetaLinux project.
  petalinux-build 

  # Force creation of the /run/media folder needed to mount MMC devices on the 
  # target system.
  echo " "
  echo "Creating /run/media within rootfs ..."
  echo " "
  mkdir ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/build/linux/rootfs/targetroot/run/media

  # Build PetaLinux project to force rootfs changes to be packaged.
  petalinux-build -c rootfs

  # Create boot image.
  petalinux-package --boot --fsbl hw_platform/${FSBL_PROJECT_NAME}.elf --fpga hw_platform/${HDL_HARDWARE_NAME}.bit --uboot --force

  # Copy the boot.bin file and name the new file BOOT_SD.bin
  cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT.BIN \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_SD.bin

  # Change to HDL scripts folder.
  cd ${START_FOLDER}/${HDL_SCRIPTS_FOLDER}

  # Clean the hardware project output products using the HDL TCL scripts.
  echo "set argv [list board=${HDL_BOARD_NAME} project=${HDL_PROJECT_NAME} clean=yes jtag=yes version_override=yes]" > cleanup.tcl
  echo "set argc [llength \$argv]" >> cleanup.tcl
  echo "source ./make.tcl -notrace" >> cleanup.tcl

  # Launch vivado in batch mode to clean output products from the hardware platform.
  vivado -mode batch -source cleanup.tcl

  # Change to PetaLinux project folder.
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/

  # Package the bitstream within the PetaLinux pre-built folder.
  petalinux-package --prebuilt --fpga hw_platform/${HDL_HARDWARE_NAME}.bit

  # Copy the template Makefile over to the PetaLinux project folder. This 
  # Makefile template can later be customized as desired. 
  cp -f ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/Makefile \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/

  # Rename the pre-built bitstream file to download.bit so that the default 
  # format for the petalinux-boot command over jtag will not need the bit file 
  # specified explicitly.
  mv -f pre-built/linux/implementation/${HDL_HARDWARE_NAME}.bit \
  pre-built/linux/implementation/download.bit

  # Change to PetaLinux projects folder.
  cd ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/

  # Copy the BOOT_SD.BIN to the pre-built images folder.
  cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_SD.bin \
  ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/pre-built/linux/images/

  # If the BOOT_QSPI_OPTION is set, copy the BOOT_QSPI.BIN to the 
  # pre-built images folder.
  if [ "$BUILD_BOOT_QSPI_OPTION" == "yes" ]
  then
    cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_QSPI.bin \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/pre-built/linux/images/
  fi

  # If the BOOT_EMMC_OPTION is set, copy the BOOT_EMMC.BIN to the 
  # pre-built images folder.
  if [ "$BUILD_BOOT_EMMC_OPTION" == "yes" ]
  then
    cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_EMMC.bin \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/pre-built/linux/images/
  fi

  # If the BOOT_EMMC_NO_BIT_OPTION is set, copy the BOOT_EMMC_No_Bit.BIN and 
  # the system.bit.bin files into the pre-built images folder.
  if [ "$BUILD_BOOT_EMMC_NO_BIT_OPTION" == "yes" ]
  then
    cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/BOOT_EMMC_No_Bit.BIN \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/pre-built/linux/images/

    cp ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/images/linux/system.bit.bin \
    ${START_FOLDER}/${PETALINUX_PROJECTS_FOLDER}/${PETALINUX_PROJECT_NAME}/pre-built/linux/images/
  fi

  # Package the hardware source into a BSP package output.
  petalinux-package --bsp -p ${PETALINUX_PROJECT_NAME} \
  --hwsource ${START_FOLDER}/${HDL_PROJECTS_FOLDER}/${HDL_PROJECT_NAME}/${HDL_BOARD_NAME}/ \
  --output ${PETALINUX_PROJECT_NAME}

  # Append the template Makefile to the PetaLinux BSP package. 
  gzip -dc ${PETALINUX_PROJECT_NAME}.bsp >${PETALINUX_PROJECT_NAME}.tar
  cp -f ${START_FOLDER}/${PETALINUX_CONFIGS_FOLDER}/Makefile .
  tar --append ${PETALINUX_PROJECT_NAME}/Makefile -f ${PETALINUX_PROJECT_NAME}.tar
  gzip -c ${PETALINUX_PROJECT_NAME}.tar > ${PETALINUX_PROJECT_NAME}.bsp
  rm -f Makefile
  rm -f ${PETALINUX_PROJECT_NAME}.tar

  # Change to PetaLinux scripts folder.
  cd ${START_FOLDER}/${PETALINUX_SCRIPTS_FOLDER}
}

main_make_function ()
{
  # This function is responsible for first creating all of the hardware
  # platforms needed for generating PetaLinux BSPs and once the hardware
  # platforms are ready, they can be specificed in HDL_BOARD_NAME variable 
  # before the call to create_petalinux_bsp.
  #
  # Once the PetaLinux BSP creation is complete, a BSP package file with the
  # name specified in the PETALINUX_PROJECT_NAME variable can be distributed
  # for use to others.

  #
  # Create the hardware platforms for the supported targets.
  #

  # Change to HDL scripts folder.
  cd ${START_FOLDER}/${HDL_SCRIPTS_FOLDER}

  # Launch vivado in batch mode to build hardware platforms for target
  # boards.
  vivado -mode batch -source make_${HDL_PROJECT_NAME}.tcl

  #
  # Create the PetaLinux BSP for the MZ7010_FMCCC target.
  #
  HDL_BOARD_NAME=MZ7010_FMCCC
  PETALINUX_PROJECT_NAME=mz_7010_2015_2_1
  create_petalinux_bsp

  #
  # Create the PetaLinux BSP for the MZ7020_FMCCC target.
  #
  HDL_BOARD_NAME=MZ7020_FMCCC
  PETALINUX_PROJECT_NAME=mz_7020_2015_2_1
  create_petalinux_bsp
}

# First source any tools scripts to setup the environment needed to call both
# PetaLinux and Vivado from this make script.
source_tools_settings

# Call the main_make_function declared above to start building everything.
main_make_function

