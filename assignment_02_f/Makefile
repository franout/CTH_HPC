CC= gcc
CFLAGS= -Wall -o
OPT= -O0
LIBS= -pthread

.PHONY: all
compile:
	$(CC) $(CFLAGS) ./newton.exe ./newton.c $(LIBS) $(OPT)
all: compile	
	./newton.exe -t 1 -l 10 5
	./newton.exe -t 2 -l 37 5
	./newton.exe -t 3 -l 1000 5
	./newton.exe -t 4 -l 1000 5
check: compile
	valgrind --leak-check=full --show-leak-kinds=all ./newton.exe
gdb:
	$(CC) $(CFLAGS) ./newton_gdb.exe ./newton.c $(LIBS) -Og -ggdb

clean:
	rm -f *.exe
