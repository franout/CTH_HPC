CC= gcc --std=c11
CFLAGS= -Wall -o
OPT= -O2
LIBS=-fopenmp -lm 
.PHONY: all
cell_distances: cell_distances.c
	$(CC) $(CFLAGS) ./cell_distances ./cell_distances.c $(LIBS) $(OPT)
all: 
	$(CC) $(CFLAGS) ./newton_gdb.exe ./newton.c $(LIBS) -Og -ggdb

execute:cell_distances
	./cell_distances -t 3

performance_e5:cell_distances 
	###################################################################	
	###number of f points 		1e4 	1e5 	1e5 	1e5 #######
	###Number of threads	 	1 	5 	10 	20  #######
	###Maximal runtime in seconds 	0.26 	9.3 	5.0 	2.7 #######
	###################################################################
	
	hyperfine -r 10 './cell_distances -t 5'
	hyperfine -r 10 './cell_distances -t 10'
	hyperfine -r 10 './cell_distances -t 20'
performance_e4:cell_distances
		hyperfine -r 10 './cell_distances -t 1'
test:  pack
	/home/hpc2019/a3_grading/check_submission.py cell_distances.tar.gz
pack: cell_distances
	tar czf cell_distances.tar.gz ./Makefile ./cell_distances.c
check_memory: cell_distances
	valgrind --leak-check=full --show-leak-kinds=all ./cell_distances
clean:
	rm -f cell_distances
	rm -f  vgcore.*
	rm -rf ./extracted
	rm -rf ./distances
