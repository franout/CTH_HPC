/*Implement a program in C, called cell_distances, that uses OpenMP for parallelization and that:

  Reads coordinates of cells from a text file “cells” in the current working directory.
  Computes the distances between cells counting how often each distance rounded to 2 decimal places occurs.
  Outputs to stdout a sorted list of distances with associated frequencies (including 0 fequencies at your will).


  Distances are symmetric, i.e. distance between two cells c_1 and c_2 and the distance in reverse direction c_2 to c_1 should be counted once.*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 
#include <unistd.h>
#include <math.h>
#define INPUT_FILE "./coordinates.txt"


typedef struct {
int * table_occ;
char ** value;
int n;
} hash_map;


static hash_map MAP;


typedef struct {
char row[25]; // supposing points between -10 and 10 with a fixed format (sign)yy.xxx 
} POINT_t;


int main (int argc , char ** argv ) {
	POINT_t point, point2;
	int N_THREAD=0;
	int option=0;
	FILE * fp,*fp_1;
	float x1,x2,y1,y2,z1,z2,dist_local;

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
	fp_1=fopen(INPUT_FILE,"r");
	if(fp==NULL || fp_1==NULL) {
		fprintf(stderr,"errror opening the file\n");
		exit(-1);
	}
	

	while ( fread(point.row,sizeof(char),24,fp)>0) {
		sscanf(point.row,"%f %f %f\n",&x1,&y1,&z1);
#pragma omp task shared(x1,y1,z1, dist_local)
		fread(point2.row,sizeof(char),24,fp);
		sscanf(point2.row,"%f %f %f\n",&x2,&y2,&z2);
		dist_local =sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(z2-z1,2));
#pragma omp taskwait 
	printf("%f\n", dist_local);
	
	}
	
	// fashion way 
//	float a = 37.777779;
//
//	int b = a; // b = 37    
//	float c = a - b; // c = 0.777779   
//	c *= 100; // c = 77.777863   
//	int d = c; // d = 77;    
//	a = b + d / (float)100; // a = 37.770000;

	fclose(fp);
	fclose(fp_1);
	if(fp==NULL) {
		fprintf(stderr,"error closing the file\n");
		exit(-1);
	}

	return 0;
}


