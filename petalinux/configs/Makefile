BOOT_FSBL_IMAGE = images/linux/zynq_fsbl.elf
BOOT_BIT_IMAGE = subsystems/linux/hw-description/system_wrapper.bit
BOOT_UBOOT_IMAGE = images/linux/u-boot.elf
BOOT_IMAGE = boot.bin
KERNEL_IMAGE = images/linux/image.ub

.PHONY: linux bootimage fsbl dts build-dts

all: linux bootimage

dts: build-dts package bootimage

rootfs: build-rootfs package bootimage

fsbl:
	petalinux-build -c bootloader

clean:
	petalinux-build -x distclean

linux:
	petalinux-build

build-dts:
	petalinux-build -c device-tree

build-rootfs:
	petalinux-build -c rootfs

package:
	petalinux-build -x package

bootimage:
	petalinux-package --boot --force --fsbl ${BOOT_FSBL_IMAGE} --fpga ${BOOT_BIT_IMAGE} --u-boot=${BOOT_UBOOT_IMAGE} -o ${BOOT_IMAGE}
