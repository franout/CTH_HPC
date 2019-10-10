/*Implement a program in C, called cell_distances, that uses OpenMP for parallelization and that:

  Reads coordinates of cells from a text file “cells” in the current working directory.
  Computes the distances between cells counting how often each distance rounded to 2 decimal places occurs.
  Outputs to stdout a sorted list of distances with associated frequencies (including 0 fequencies at your will).


  Distances are symmetric, i.e. distance between two cells c_1 and c_2 and the distance in reverse direction c_2 to c_1 should be counted once.*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 
#define INPUT_FILE "./coordinates.txt"
int main (int argc , char ** argv ) {

	int N_THREAD=0;
	int option=0;
	FILE * fp;
	/*it returns the parsed char and as third arg it requires the separation char for arguments*/
	while ((option = getopt(argc,argv,"t:"))!=-1) {

		switch(option){

			case 't':
				sscanf(optarg,"%d",&N_THREAD);		
				break;
			default:
				fprintf(stdout,"no valid arguments\n");
				exit(-1);

		}
	}
	omp_set_num_threads(N_THREAD);

	fp=fopen(INPUT_FILE,"r");
	if(fp==NULL) {
		fprintf(stderr,"errror opening the file\n");
		exit(-1);
	}


	//openMP parallelism  
	/*4 task
	 *
	 *
	 *     Computing the distance of two points and incrementing the corresponding counts.
	 *
	 *         Reading and parsing the file.
	 *
	 *             Memory management.
	 *
	 *                 Parallization.
	 *
	 *
	 * */




// fashion way 
//	float a = 37.777779;
//
//	int b = a; // b = 37    
//	float c = a - b; // c = 0.777779   
//	c *= 100; // c = 77.777863   
//	int d = c; // d = 77;    
//	a = b + d / (float)100; // a = 37.770000;



	fclose(fp);
	if(fp==NULL) {
		fprintf(stderr,"error closing the file\n");
		exit(-1);
	}

	return 0;
}

/*
   Implementation details: Simplifications

   You can assume that coordinates are between -10 and 10.

   Implementation details: Memory consition

   The programm at no time may consume more than 1 GiBi byte = 1024^3 bytes of memory. This will not be tested. Explain in your report why your program does not allocate more.

   You may not make any assumption on the number of cells except that there is less than 2^32.

   You do not have to proof that this is the case, but your report has to contain an arugment why your implementation achieves this.
   Implementation details: Precision

   Round off error are tolerated to a certain extend.

   In case of doubt use the test script from below.*//
