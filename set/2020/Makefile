CC=gcc
CFLAGS=-O2 -lm -fopenmp -Wall -Wextra -pedantic

SRCS=$(wildcard ex*.c)
PROGS=$(SRCS:.c=)

.PHONY: all, clean

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) a.out
	$(RM) $(PROGS)
