CC= gcc
CFLAGS= -Wall -o
OPT= -O2
LIBS= -pthread -lm

.PHONY: all
newton: newton.c
	$(CC) $(CFLAGS) ./newton ./newton.c $(LIBS) $(OPT)
all: newton
	
	./newton.exe -t 1 -l 10 5
	./newton.exe -t 2 -l 37 5
	./newton.exe -t 3 -l 1000 5
	./newton.exe -t 4 -l 1000 5

performance: newton
	##### single thread varialbe d
	
	hyperfine -r 10 './newton -t 1 -l 1000 1'
	hyperfine -r 10 './newton -t 1 -l 1000 2'
	hyperfine -r 10 './newton -t 1 -l 1000 5'	
	hyperfine -r 10 './newton -t 1 -l 1000 7'
	
	
	##multithreads
	hyperfine -r 10 './newton -t 1 -l 1000 5'
	hyperfine -r 10 './newton  -t 2 -l 1000 5'
	hyperfine -r 10 './newton  -t 3 -l 1000 5'
	hyperfine -r 10 './newton  -t 4 -l 1000 5'
	##  varyng numbers of lines
	hyperfine -r 10 './newton -t 10 -l 1000 7'
	hyperfine -r 10 './newton  -t 10 -l 2000 7'
	hyperfine -r 10 './newton  -t 10 -l 3000 7'
	make clean	

check: newton 
	valgrind --leak-check=full --show-leak-kinds=all ./newton

check_submission: check pack
	/home/hpc2019/a2_grading/check_submission.py ./newton.tar.gz
pack:newton
	tar czf newton.tar.gz  ./Makefile ./newton.c 
gdb:
	$(CC) $(CFLAGS) ./newton_gdb.exe ./newton.c $(LIBS) -Og -ggdb

clean:
	rm -f *.exe
	rm -f *.ppm
	rm -rf ./pictures
	rm -rf ./extracted
	rm -f ./newton

