ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

APP = ultrazed_iocc_sensor_init.sh

all: build install

build: $(APP)

clean:

.PHONY: install image

install: $(APP)
	$(TARGETINST) -d -p 0755 $(APP) /etc/init.d/$(APP)
	$(TARGETINST) -s /etc/init.d/$(APP) /etc/rc5.d/S99_startup



