CC=gcc
CFLAGS=-Wall
OUT=hts221_sensor
OBJS=hts221_sensor.o

$(OUT): hts221_sensor.o
        $(CC) $(CFLAGS) -o $(OUT) $(OBJS)

hts221_sensor.o: hts221_sensor.c
        $(CC) $(CFLAGS) -c hts221_sensor.c -o hts221_sensor.o

.PHONY: clean

clean:
        rm -rf *.o $(OUT)
