ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

APP = weaved
PKG = weavedOEM

# Add any other object files to this list below
APP_OBJS = weaved.o

all: build install

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

clean:
	-rm -f $(APP) *.elf *.gdb *.o

.PHONY: install image

install: $(APP)
#	$(TARGETINST) -d $(APP) /home/root/$(APP)
	$(TARGETINST) -d -p 0755 $(PKG) /home/root/$(PKG)
# The following simlink is needed under petalinux 2016.2 for weaved to work
	$(TARGETINST) -s /lib/ld-2.19-2014.08.so /lib/ld-linux.so.3

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  clean                  clean out build objects"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  build                  build subsystem"
	@echo "  install                install built objects to rootfs host copy"
