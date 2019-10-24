.PHONY : all
PATH_ARB_I=$(PWD)/./../local_arb/include
PATH_FLINT_I=$(PWD)/./../local_flint/include
PATH_ARB_L=$(PWD)/../local_arb/lib
PATH_FLINT_L=$(PWD)/../local_flint/lib

all :  main
	
main : 
	gcc  -Wall -o poly_roots.exe  poly_roots.c -I$(PATH_ARB_I) -L$(PATH_ARB_L) -larb -I$(PATH_FLINT_I) -L$(PATH_FLINT_L)  -lflint
	ldd ./poly_roots.exe ; # showing up the dependacies of the libraries

main22: obj
	gcc -Wall -o poly_roots.exe  poly_roots.o  -I$(PATH_ARB_I) -L$(PATH_ARB_L) -larb -I$(PATH_FLINT_I) -L$(PATH_FLINT_L) -lflint


obj: poly_roots.c
	gcc -c poly_roots.c  -I$(PATH_ARB_I) -L$(PATH_ARB_L) -lflint -I$(PATH_FLINT_I) -L$(PATH_FLINT_L) -lflint 
	
main2: libflint.so libarb.so
	unset LD_LIBRARY_PATH
	gcc  -Wall -o poly_roots.exe -larb -lflint poly_roots.c -I$(PATH_ARB_I) -L$(PATH_ARB_L) -I$(PATH_FLINT_I) -L$(PATH_FLINT_L)
	ldd ./poly_roots.exe ; # showing up the dependacies of the libraries 
	

libflint.so :
	gcc -o $(PATH_FLINT_L)/libflint.so -shared -fpic

libarb.so : libflint.so
	gcc -o $(PATH_ARB_L)/libarb.so -shared -fpic
