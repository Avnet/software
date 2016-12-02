ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

APP = linux_eeprom_test.sh


# Add any other object files to this list below
APP_OBJS = $(APP).o

all: build install

build: $(APP)

clean:

.PHONY: install image

install: $(APP)
	$(TARGETINST) -p 0755 $(APP) /home/root/$(APP)

%.o: %.sh



