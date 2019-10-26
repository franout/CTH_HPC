CC = gcc
CFLAGS =  -O2 -march=native
OCL = -lOpenCL -lm

.PHONY: all clean test

all: heat_diffusion

heat_diffusion: heat_diffusion.c
	$(CC) $(CFLAGS) $(OPT) -o heat_diffusion heat_diffusion.c $(OCL)
heat_diffusion_cpu: heat_diffusion_cpu.c
	$(CC) $(CFLAGS) $(OPT) -o ./heat_diffusion_cpu ./heat_diffusion_cpu.c
heat_diffusion_no_red:heat_diffusion_no_red.c
	$(CC)  $(CFLAGS) $(OPT) -o heat_diffusion_no_red heat_diffusion_no_red.c $(OCL)
execute: heat_diffusion
	./heat_diffusion -d 0.03333 -n 2
clean:
	rm -f *.out heat_diffusion heat_diffusion_cpu heat_diffusion_no_red
performance_cpu: heat_diffusion_cpu copy_files
	###################################################################################
	#### Width * Height 		100 * 100 	10000 * 10000 	100000 * 100 ######
	#### Diffusion constant 	0.01 		0.02 		0.6	     ######
	#### Number of iterations 	100000 		1000 		200	     ######
	#### Time in seconds 	 	1.65 		97.6 		1.4	     ######
	###################################################################################
	mv ./diffusion_100_100 ./diffusion
	hyperfine -r 10 './heat_diffusion_cpu -n 100000 -d 0.01'
	mv ./diffusion ./diffusion_100_100
	mv ./diffusion_10000_10000 ./diffusion 
	hyperfine -r 10 './heat_diffusion_cpu -n 1000 -d 0.02'
	mv ./diffusion ./diffusion_10000_10000
	mv ./diffusion_100000_100 ./diffusion
	hyperfine -r 10 './heat_diffusion_cpu -n 200 -d 0.6'
	mv ./diffusion ./diffusion_100000_100
performance_no_reduction:heat_diffusion_no_red copy_files
	###################################################################################
	#### Width * Height 		100 * 100 	10000 * 10000 	100000 * 100 ######
	#### Diffusion constant 	0.01 		0.02 		0.6	     ######
	#### Number of iterations 	100000 		1000 		200	     ######
	#### Time in seconds 	 	1.65 		97.6 		1.4	     ######
	###################################################################################
	mv ./diffusion_100_100 ./diffusion
	hyperfine -r 10 './heat_diffusion_no_red -n 100000 -d 0.01'
	mv ./diffusion ./diffusion_100_100
	mv ./diffusion_10000_10000 ./diffusion 
	hyperfine -r 10 './heat_diffusion_no_red -n 1000 -d 0.02'
	mv ./diffusion ./diffusion_10000_10000
	mv ./diffusion_100000_100 ./diffusion
	hyperfine -r 10 './heat_diffusion_no_red -n 200 -d 0.6'
	mv ./diffusion ./diffusion_100000_100
performance: heat_diffusion copy_file copy_files
	###################################################################################
	#### Width * Height 		100 * 100 	10000 * 10000 	100000 * 100 ######
	#### Diffusion constant 	0.01 		0.02 		0.6	     ######
	#### Number of iterations 	100000 		1000 		200	     ######
	#### Time in seconds 	 	1.65 		97.6 		1.4	     ######
	###################################################################################
	mv ./diffusion_100_100 ./diffusion
	hyperfine -r 10 './heat_diffusion -n 100000 -d 0.01'
	mv ./diffusion ./diffusion_100_100
	mv ./diffusion_10000_10000 ./diffusion 
	hyperfine -r 10 './heat_diffusion -n 1000 -d 0.02'
	mv ./diffusion ./diffusion_10000_10000
	mv ./diffusion_100000_100 ./diffusion
	hyperfine -r 10 './heat_diffusion -n 200 -d 0.6'
	mv ./diffusion ./diffusion_100000_100
test: pack 
	/home/hpc2019/a4_grading/check_submission.py ./heat_diffusion.tar.gz

pack: heat_diffusion
	tar czf ./heat_diffusion.tar.gz ./heat_diffusion.c ./makefile ./heat_diffusion.cl

copy_files:
	rm -f ./diffusion
	cp /home/hpc2019/a4_grading/test_data/* ./

