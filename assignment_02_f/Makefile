CC= gcc
CFLAGS= -Wall -o
OPT= -O2
LIBS= -pthread -lcomplex

.PHONY: all
compile:
	$(CC) $(CFLAGS) ./newton.exe ./newton.c $(LIBS) $(OPT)
all: compile
	
	./newton.exe -t 1 -l 10 5
	./newton.exe -t 2 -l 37 5
	./newton.exe -t 3 -l 1000 5
	./newton.exe -t 4 -l 1000 5
check: compile_O0
	valgrind --leak-check=full --show-leak-kinds=all ./newton.exe

check_submission: check pack
	/home/hpc2019/a2_grading/check_submission.py ./newton.tar.gz
pack:	compile
	tar czf newton.tar.gz ./newton.exe ./newton.c
compile_O0:
	$(CC) $(CFLAGS) ./newton.exe ./newton.c $(LIBRS) $(OPT)
gdb:
	$(CC) $(CFLAGS) ./newton_gdb.exe ./newton.c $(LIBS) -Og -ggdb

clean:
	rm -f *.exe
	rm -f *.ppm
