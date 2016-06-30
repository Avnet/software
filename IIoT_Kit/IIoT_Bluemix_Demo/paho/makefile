CC = gcc
CFLAGS =
AR = ar
AROPTS = rcs
OUT = libpaho.a
SRCS := $(wildcard *.c)
OBJS := ${SRCS:.c=.o}
DEPS := $(wildcard *.h)


$(OUT): $(OBJS)
	$(AR) $(AROPTS) $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf *.o $(OUT)
