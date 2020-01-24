CC=gcc
CFLAGS=-O2 -lm -fopenmp

SRCS=$(wildcard *.c)
PROGS=$(SRCS:.c=)

.PHONY: all, clean

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm $(PROGS)
