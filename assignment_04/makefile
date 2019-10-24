CC = gcc
CFLAGS =  -O2 -march=native
OCL = -lOpenCL -lm

.PHONY: all clean test

all: heat_diffusion

heat_diffusion: heat_diffusion.c
	$(CC) $(CFLAGS) $(OPT) -o heat_diffusion heat_diffusion.c $(OCL)
execute: heat_diffusion
	./heat_diffusion -d 0.03333 -n 2
clean:
	rm -f *.out heat_diffusion

performance: heat_diffusion
	###################################################################################
	#### Width * Height 		100 * 100 	10000 * 10000 	100000 * 100 ######
	#### Diffusion constant 	0.01 		0.02 		0.6	     ######
	#### Number of iterations 	100000 		1000 		200	     ######
	#### Time in seconds 	 	1.65 		97.6 		1.4	     ######
	###################################################################################
	#there is a probles somewhere
	hyperfine -r 10  './heat_diffusion -n100000 -d0.01'
	mv diffusion ./diffusion_100_100
	mv diffusion_10000_1000 ./diffusion
	hyperfine -r 10 './heat_diffusion -d 0.02 -n 1000'
	mv diffusion ./diffusion_10000_10000
	mv diffusion_100000_100 ./diffusion
	hyperfine -r 10 './heat_diffusion -d 0.6 -n 200'
	mv ./diffusion ./diffusion_100000_100
test: pack 
	/home/hpc2019/a4_grading/check_submission.py ./heat_diffusion.tar.gz

pack: heat_diffusion
	tar czf ./heat_diffusion.tar.gz ./heat_diffusion.c ./makefile ./heat_diffusion.cl
