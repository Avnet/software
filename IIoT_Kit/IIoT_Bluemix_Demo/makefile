CC = gcc
CFLAGS =
OUT = IIoT_Bluemix_Demo
SRCS := $(wildcard *.c)
OBJS := ${SRCS:.c=.o}
DEPS := $(wildcard *.h)
SUBDIR = ./paho
INCDIR = -I$(SUBDIR)
LIB = -lpaho
LIBDIR += -L$(SUBDIR)


all: $(OBJS) paho
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS) $(LIBDIR) $(LIB)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@ $(LIBDIR) $(LIB)

.PHONY: clean paho

paho:
	$(MAKE) -C $(SUBDIR)

clean:
	rm -rf *.o $(OUT)
	$(MAKE) -C $(SUBDIR) clean

