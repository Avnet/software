CC=gcc
CFLAGS=-Wall
OUT=maxim31855_sensor
OBJS=maxim31855.o maxim.o
DEPS=PmodUtilities.h

$(OUT): maxim31855.o maxim.o
        $(CC) $(CFLAGS) -o $(OUT) $(OBJS)

maxim31855.o: maxim31855.c $(DEPS)
        $(CC) $(CFLAGS) -c maxim31855.c -o maxim31855.o

maxim.o: maxim.c $(DEPS)
        $(CC) $(CLFAGS) -c maxim.c -o maxim.o

.PHONY: clean

clean:
        rm -rf *.o $(OUT)
