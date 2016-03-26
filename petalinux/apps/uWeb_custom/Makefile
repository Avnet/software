ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

all: build install

build:

clean:

.PHONY: install image

install:
	$(TARGETINST) -d -p 0555 sp605-2.png /home/httpd/img/sp605-2.png

help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  install                install built objects to rootfs host copy"
