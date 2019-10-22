CC=gcc
CFLAGS= -Wall -o

.PHONY: all 

all: time inlining locality indirect_addressing writing_to_disk valgrind gdb
time:
	#################################################################################################
	## produce programs with different optimization level and also the correspondent assembly code ##
	#################################################################################################
	###########
	## opt 0 ##
	###########
	$(CC)  $(CFLAGS) $@_O0.s $@.c -S -O0
	$(CC) $(CFLAGS) $@_O0.exe $@_O0.s -O0
	./$@_O0.exe 
	###########
	## opt 1 ##
	###########
	$(CC) $(CFLAGS) $@_O1.s $@.c -S -O1
	$(CC) $(CFLAGS) $@_O1.exe $@_O1.s -O1
	./$@_O1.exe
	###########
	## opt 2 ##
	###########
	$(CC)  $(CFLAGS) $@_O2.s $@.c -S -O2
	$(CC) $(CFLAGS) $@_O2.exe $@_O2.s -O2
	./$@_O2.exe 
	###########
	## opt 3 ##
	###########
	$(CC) $(CFLAGS) $@_O3.s $@.c -S -O3
	$(CC) $(CFLAGS) $@_O3.exe $@_O3.s -O3
	./$@_O3.exe 
	############################
	## opt f -> for fast code ##
	############################
	$(CC) $(CFLAGS) $@_Of.s $@.c -S -Ofast
	$(CC) $(CFLAGS) $@_Of.exe $@_Of.s -Ofast
	./$@_Of.exe 
	############################
	## opt s -> opt for space ##
	############################
	$(CC) $(CFLAGS) $@_Os.s $@.c -S -Os
	$(CC) $(CFLAGS) $@_Os.exe $@_Os.s -Os
	./$@_Os.exe 
	#############################################################
	## opt g -> optimize for a better debug experience than O0 ##
	#############################################################
	$(CC) $(CFLAGS) $@_Og.s $@.c -S -Og
	$(CC) $(CFLAGS) $@_Og.exe $@_Og.s -Og
	./$@_Og.exe 

inlining:
	$(CC)  $(CFLAGS) ./$@.exe ./inlined.c -O2
	./$@.exe 
	$(CC)  $(CFLAGS)  ./$@_mf.exe ./inlined_mainfile.c -O2
	./$@_mf.exe  
	$(CC)  $(CFLAGS)  ./$@_sf.exe ./inlined_separatefile.c ./mylib.h ./mylib.c -O2
	./$@_sf.exe 
	$(CC) $(CFLAGS) ./$@_stinl.exe ./inlined_static.c -O2
	./$@_stinl.exe
	####################################################################################################
	################################ inspecting symbols in inlining_main file ##########################
	####################################################################################################
	nm ./$@_mf.exe | grep mul_cpx -A 3 -B 3
	####################################################################################################
	############################## inspecting symbols in inlining_separate file ########################
	####################################################################################################
	nm ./$@_sf.exe | grep mul_cpx -A 3 -B 3
	#######################################################################################################
	############################## inspecting symbols in inlining_static inlining  ########################
	#######################################################################################################
	nm ./$@_stinl.exe | less 
locality:
	$(CC) $(CFLAGS)  ./$@_O0.exe $@.c -O0
	./$@_O0.exe
	$(CC) $(CFLAGS) ./$@_O2.exe $@.c -O2
	./$@_O2.exe


indirect_addressing:
	$(CC) $(CFLAGS)  ./$@_O0.exe $@.c -O0
	./$@_O0.exe
	$(CC) $(CFLAGS) ./$@_O2.exe $@.c -O2
	./$@_O2.exe

writing_to_disk:
	$(CC) $(CFLAGS) ./$@.exe ./writing_hddssd.c -O2
	./$@.exe
	
	cp -rf /usr/include/ ~/my_include
	cp -rf /usr/include/ /run/mount/scratch/hpcuser018/my_include
	hyperfine -r 10  "cp -rf ~/my_include/ ~/include_copy/ "
	hyperfine  -r 10 "cp -rf /run/mount/scratch/hpcuser018/my_include/ /run/mount/scratch/hpcuser018/include_copyC/	"

valgrind:
	$(CC) $(CFLAGS) ./$@.exe ./valgrind_tutorial.c -O0 -ggdb
	valgrind --leak-check=full ./$@.exe
	echo "" > tmp_vt.c
	###################################
	## removing initialization of as ##
	###################################
	regexp='int.+\*.*'; \
	 while  read -r line ; \
	do \
	if [[  $$line =~ $$regexp  ]] ; then \
		echo "int * as;" >> tmp_vt.c ; \
	 	else \
		echo "$$line" >> tmp_vt.c ; \
	       	fi ; \
	       done < valgrind_tutorial.c   
	$(CC)  $(CFLAGS) ./$@.exe tmp_vt.c -O0 -ggdb
	valgrind --leak-check=full ./$@.exe &
	sleep 3	
	rm -f tmp_vt.c
	#########################
	## removing free of as ##
	#########################
	regexp='.*free\(.*' ; \
	 while  read -r line ; \
	do \
	if [[  $$line =~ $$regexp  ]] ; then \
		echo " " >> tmp_vt.c ; \
	 	else \
		echo "$$line" >> tmp_vt.c ; \
	       	fi ; \
	       done < valgrind_tutorial.c 
	$(CC)  $(CFLAGS) ./$@.exe tmp_vt.c -O0 -ggdb
	valgrind --leak-check=full ./$@.exe &
	sleep 3
	rm -f tmp_vt.c
	################################
	## adding one more free of as ##
	################################
	regexp='.*free\(.*'; \
	 while  read -r line ; \
	do \
	if [[  $$line =~ $$regexp  ]] ; then \
		echo "free(as); free(as);" >> tmp_vt.c ; \
	 	else \
		echo "$$line" >> tmp_vt.c ; \
	       	fi ; \
	       done < valgrind_tutorial.c ;  \
	$(CC)  $(CFLAGS) ./$@.exe tmp_vt.c -O0 -ggdb
	valgrind --leak-check=full ./$@.exe &
	sleep 3 
	rm -f tmp_vt.c
gdb:
	$(CC) $(CFLAGS)   ./gdb_tutorial_O0.exe ./gdb_tutorial.c -O0 -ggdb ; # additional debug information with -ggdb same as -g
	$(CC) $(CFLAGS)  ./gdb_tutorial_Og.exe ./gdb_tutorial.c -Og -ggdb

clean:
	rm -f *.exe
	rm -f *.s
	rm -f vgcore*
	rm -f tmp*
	rm -Rf  /run/mount/scratch/hpcuser018/my_include/
	rm -Rf  /run/mount/scratch/hpcuser018/include_copyC/
	rm -Rf ~/my_include/
	rm -Rf ~/include_copy/
	
