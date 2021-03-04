CC=gcc
CFLAGS=-Wall -DDEBUG
LDLIBS=-lpthread

.PHONY: clean

ex4: ex4.o ex4_driver.o traffic_synchronizer.h

clean:
	rm ex4.o ex4_driver.o ex4

.PHONY: all
all: ex4
