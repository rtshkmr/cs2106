CC=gcc
CFLAGS=-Wall
LDLIBS=-lpthread

.PHONY: clean

ex2: ex2.o ex2_driver.o rw_lock.h

clean:
	rm ex2.o ex2_driver.o ex2

.PHONY: all
all: ex2
