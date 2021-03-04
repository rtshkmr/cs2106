CC=gcc
CFLAGS=-Wall
LDLIBS=-lpthread

.PHONY: clean

ex1: barrier.o ex1.o

barrier.o: barrier.c barrier.h

clean:
	rm barrier.o ex1.o ex1

.PHONY: all
all: ex1
