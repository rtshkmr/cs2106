CC=gcc
CFLAGS=-Wall
LDLIBS=-lpthread

.PHONY: clean

ex3: ex3.o ex3_driver.o rw_lock.h

clean:
	rm ex3.o ex3_driver.o ex3

.PHONY: all
all: ex3
