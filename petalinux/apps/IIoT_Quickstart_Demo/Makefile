ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

APP = IIoT_Quickstart_Demo

# Add any other object files to this list below
APP_OBJS = ./src/MQTTClient.o \
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
	./src/iotfclient.o \
	./src/maxim31855.o \
	./src/readhts221.o \
	./src/main.o

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


#CC = gcc
#CFLAGS =
#OUT = IIoT_Quickstart_Demo
#SRCS := $(wildcard *.c)
#OBJS := ${SRCS:.c=.o}
#DEPS := $(wildcard *.h)
#SUBDIR = ./src/paho
#INCDIR = -I$(SUBDIR)
#LIB = -lpaho
#LIBDIR += -L$(SUBDIR)

#all: build install

#build: $(APP)

#$(APP): $(OBJS) paho
#	$(CC) $(CFLAGS) -o $(OUT) $(OBJS) $(LIBDIR) $(LIB)

#%.o: %.c $(DEPS)
#	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@ $(LIBDIR) $(LIB)

#.PHONY: install image

#install: $(APP)
#	$(TARGETINST) -d -p 0755 $(APP) /home/root/$(APP)

#paho:
#	$(MAKE) -C $(SUBDIR)

#clean:
#	rm -rf *.o $(OUT)
#	$(MAKE) -C $(SUBDIR) clean

help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  clean                  clean out build objects"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  build                  build subsystem"
	@echo "  install                install built objects to rootfs host copy"
