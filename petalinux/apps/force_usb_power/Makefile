ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

all: build install

build:

clean:

.PHONY: install image

install:
	$(TARGETINST) -d -p 0755 force_usb_power.sh /etc/init.d/force_usb_power.sh
	$(TARGETINST) -s ../init.d/force_usb_power.sh /etc/rc5.d/S99force_usb_power

help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  install                install built objects to rootfs host copy"
