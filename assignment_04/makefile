CC = gcc
CFLAGS = -O2 -march=native
OCL =

.PHONY: all clean test

all: heat_diffusion

heat_diffusion: heat_diffusion.c
	$(CC) $(CFLAGS) $(OPT) -o heat_diffusion heat_diffusion.c

clean:
	rm -f *.out heat_diffusion

performance: heat_diffusion
	hyperfine --warmup="10" heat_diffusion

test: pack 
	/home/hpc2019/a4_grading/check_submission.py ./heat_diffusion.tar.gz

pack: heat_diffusion
	tar czf ./heat_diffusion.tar.gz ./heat_diffusion.c ./makefile
