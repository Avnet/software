CC = gcc
CFLAGS = -Wall
OUT = IIoT_Bluemix_Demo
SRCS := $(wildcard *.c)
OBJS := ${SRCS:.c=.o}
DEPS := $(wildcard *.h)


$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf *.o $(OUT)
