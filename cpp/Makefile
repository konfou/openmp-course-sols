CC=g++
CFLAGS=-O2 -lm -fopenmp -Wall -Wextra -pedantic

SRCS=$(wildcard ex*.cpp)
PROGS=$(SRCS:.cpp=)

.PHONY: all, clean

all: $(PROGS)

%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) a.out
	$(RM) $(PROGS)
