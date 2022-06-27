CC=gcc
CFLAGS=-lm -fopenmp -Wall -Wextra -pedantic

SRCS=$(wildcard ex*.c)
PROGS=$(SRCS:.c=)

.PHONY: all, clean

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< validate.c

clean:
	$(RM) *dat
	$(RM) $(PROGS)
