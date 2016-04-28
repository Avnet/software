ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

APP = linux_temperature_client.elf

# Add any other object files to this list below
APP_OBJS = ./src/iotfclient.o \
	./src/MQTTClient.o \
	./src/MQTTConnectClient.o \
	./src/MQTTConnectServer.o \
	./src/MQTTDeserializePublish.o \
	./src/MQTTFormat.o \
	./src/MQTTLinux.o \
	./src/MQTTPacket.o \
	./src/MQTTSerializePublish.o \
	./src/MQTTSubscribeClient.o \
	./src/MQTTSubscribeServer.o \
	./src/MQTTUnsubscribeClient.o \
	./src/MQTTUnsubscribeServer.o \
	./src/temperature_client.o \
	./src/temperature_sensor.o

all: build install

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

clean:
	-rm -f $(APP) *.elf *.gdb *.o

.PHONY: install image

install: $(APP)
	$(TARGETINST) -d -p 0755 $(APP) /home/root/$(APP)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  install                install built objects to rootfs host copy"

