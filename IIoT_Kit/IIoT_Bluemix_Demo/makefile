CC = gcc
CFLAGS = -Wall
OUT = IIoT_Bluemix_Demo
SRCS := $(wildcard *.c)
OBJS := ${SRCS:.c=.o}
DEPS := $(wildcard *.h)
INCDIR = -I./paho
LIB += -lpaho
LIBDIR += -L./paho

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS) $(LIBDIR) $(LIB)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@ $(LIBDIR) $(LIB)

.PHONY: clean

clean:
	rm -rf *.o $(OUT)
