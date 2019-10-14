CC= gcc --std=c11
CFLAGS= -Wall -o
OPT= -O2
LIBS=-fopenmp -lm 

.PHONY: all
compile: cell_distances.c
	$(CC) $(CFLAGS) ./cell_distances ./cell_distances.c $(LIBS) $(OPT)
all: compile
	$(CC) $(CFLAGS) ./newton_gdb.exe ./newton.c $(LIBS) -Og -ggdb

execute:compile
	./cell_distances -t 3

performace: 
	###################################################################	
	###number of f points 	1e4 	1e5 	1e5 	1e5	    #######
	###Number of threads 	1 	5 	10 	20          #######
	###Maximal runtime in seconds 	0.26 	9.3 	5.0 	2.7 #######
	###################################################################
	echo "" > tmp_cd.c
	regexp='#define INPUT_FILE .*'; \
	 while  read -r line ; \
	do \
	if [[  $$line =~ $$regexp  ]] ; then \
		echo "#define INPUT_FILE "./cell_e4"" >> tmp_cd.c ; \
	 	else \
		echo "$$line" >> tmp_cd.c ; \
	       	fi ; \
	       done <  ./cell_distances.c
	more tmp_cd.c > cell_distances.c
	make compile
	hyperfine -r 10 './cell_distances -t 1' 
	
	echo "" > tmp_cd.c
	regexp='#define INPUT_FILE .*'; \
	 while  read -r line ; \
	do \
	if [[  $$line =~ $$regexp  ]] ; then \
		echo "#define INPUT_FILE "./cell_e5" " >> tmp_cd.c ; \
		else \
		echo "$$line" >> tmp_cd.c ; \
	       	fi ; \
	       done <  ./cell_distances.c
	more tmp_cd.c > cell_distances.c
	make compile
	hyperfine -r 10 './cell_distances -t 5'
	hyperfine -r 10 './cell_distances -t 10'
	hyperfine -r 10 './cell_distances -t 20'
test: compile pack
	/home/hpc2019/a3_grading/check_submission.py cell_distances.tar.gz
pack: compile
	tar czf cell_distances.tar.gz ./cell_distances.exe ./cell_distances.c

clean:
	rm -f *.exe

