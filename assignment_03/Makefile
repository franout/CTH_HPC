CC= gcc --std=c11
CFLAGS= -Wall -o
OPT= -O0
LIBS=-fopenmp -lm 

.PHONY: all
compile:
	$(CC) $(CFLAGS) ./cell_distances.exe ./cell_distances.c $(LIBS) $(OPT)
all: compile
	$(CC) $(CFLAGS) ./newton_gdb.exe ./newton.c $(LIBS) -Og -ggdb


execute:
		
test: compile pack
	./cell_distances.exe  > out.txt
	/home/hpc2019/a3_grading/check_submission.py cell_distances.tar.gz
pack: 
	tar czf cell_distances.tar.gz ./cell_distances.exe ./cell_distances.c

clean:
	rm -f *.exe

