CC=g++
CFLAGS=-lm -fopenmp -Wall -Wextra -pedantic

SRCS=$(wildcard ex*.cpp)
PROGS=$(SRCS:.cpp=)

.PHONY: all, clean

all: $(PROGS)

%: %.cpp
	$(CC) $(CFLAGS) -o $@ $< validate.cpp

clean:
	$(RM) *dat
	$(RM) $(PROGS)
