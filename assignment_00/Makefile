.PHONY: all
STACK_SIZE=10
CC=gcc
CFLAGS=-Wall -o
all: stack_allocation heap_allocation memory_fragmentation writing_files parsing_command_line_arguments

stack_allocation:
	#first compilation with a stack size of 10
	
	stack_size=$(STACK_SIZE); flag=0 ; regexp='#define .*';  \
	while [ $$flag -eq 0  ] ; \
	do  \
	 while  read -r line ; \
	do \
	if [[  $$line =~ $$regexp  ]] ; then \
		echo "#define SIZE $$stack_size" >> tmp.sa ; \
	 	else \
		echo $$line >> tmp.sa ; \
	       	fi ; \
	       done < stack_allocation.c ;  \
	mv tmp.sa stack_allocation.c ;\
	rm -f tmp.sa	      ;\
	$(CC)  -O1 $(CFLAGS) stack_allocation.exe stack_allocation.c ;\
	./stack_allocation.exe ;\
	flag=$$? ; \
	stack_size=$$((  $$stack_size*10 )) ; \
	echo "compilation with stack size = $$stack_size " ; \
	done ; \
	echo "Segmentation fault is reached with a stack size = $$stack_size"

heap_allocation: stack_allocation
	./change_size.sh ./heap_allocation.c $(STACK_SIZE)
	$(CC) -O1 $(CFLAGS) ./heap_allocation.exe ./heap_allocation.c
	./heap_allocation.exe

memory_fragmentation:
	$(CC) $(CFLAGS) ./memory_fragmentation.exe ./memory_fragmentation.c
	./memory_fragmentation.exe
	$(CC) $(CFLAGS) ./no_memory_fragmentation.exe ./no_memory_fragmentation.c
	./no_memory_fragmentation.exe
writing_files:
	$(CC) $(CFLAGS) ./writing_files.exe ./writing_files.c
	./writing_files.exe

parsing_command_line_arguments:
	$(CC) $(CFLAGS) ./$@.exe ./parsing_args.c
	./$@.exe -a2 -b4
	./$@.exe -b4 -a2
	./$@.exe -a 2 -b4
clean: 
	rm -f *.exe
