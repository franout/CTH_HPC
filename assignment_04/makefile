CC = gcc
CFLAGS = -O2 -march=native
OCL =

.PHONY: all clean test

all: heat_diffusion

heat_diffusion: heat_diffusion.c
	$(CC) $(CFLAGS) $(OPT) -o heat_diffusion heat_diffusion.c

clean:
	rm -f *.out heat_diffusion

test:
	$(CC) $(CFLAGS) $(OPT) -o heat_diffusion heat_diffusion.c
	hyperfine --warmup="10" heat_diffusion
