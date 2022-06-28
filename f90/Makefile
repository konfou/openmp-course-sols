FC=gfortran
FFLAGS=-lm -fopenmp -Wall -Wextra -pedantic

SRCS=$(wildcard ex*.f90)
PROGS=$(SRCS:.f90=)

.PHONY: all, clean

all: $(PROGS)

%: %.f90
	$(FC) $(FFLAGS) -o $@ $<

clean:
	$(RM) *dat
	$(RM) $(PROGS)
