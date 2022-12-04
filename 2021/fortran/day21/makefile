.SUFFIXES:
FC=gfortran
FCFLAGS= -g -O2 -Wall -Werror
SRC=$(wildcard *.f90)
OBJ=$(SRC:.f90=.o)
EXE=$(SRC:.f90=.exe)

all: $(EXE)

%.exe : %.o
	$(FC) -o $*.exe $<
	@touch $@

%.o %.mod %.smod: %.f90
	$(FC) $(FCFLAGS) -c $<
	@touch $@

.PHONY: clean all
clean:
	rm -f *.o *.mod *.exe

.PRECIOUS: %.o
