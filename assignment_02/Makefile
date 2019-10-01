CC= gcc
CFLAGS= -Wall -o
LIBS= -pthread

-PHONY: all


all:
	$(CC) $(CFLAGS) ./newton.exe ./newton.c $(LIBS)
	valgrind --leak-check=full ./.newton.c
	./newton.c -t 1 -l 10 2


clean:
	rm -f *.exe
