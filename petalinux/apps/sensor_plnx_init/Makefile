ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

APP = plnx_init

# Add any other object files to this list below
APP_OBJS = plnx_init.o

all: build install

build: $(APP)

clean:

.PHONY: install image

install: $(APP)
	$(TARGETINST) -d -p 0755 $(APP) /etc/init.d/$(APP)
	$(TARGETINST) -s /etc/init.d/$(APP) /etc/rc5.d/S99mystartup

%.o: %.sh

help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  clean                  clean out build objects"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  build                  build subsystem"
	@echo "  install                install built objects to rootfs host copy"
