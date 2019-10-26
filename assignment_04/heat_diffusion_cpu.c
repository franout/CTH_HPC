#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define CL_KERNEL_FILE "./heat_diffusion.cl"
#define INPUT_FILE "./diffusion"
#define MAX_KERNEL_SIZE 1048576 // 1 Kbytes
#define ARRAY_MATRIX 1
#define DEBUG 0

int main (int argc , char ** argv ) 
{
	int required_iterations=0;
	float diffusion_const=0;
	int ret;
	int option=0;
	int h,w,x,y;
#ifndef ARRAY_MATRIX
	float  ** matrix;
#else 
	float *matrix;
#endif
	float val,avg;
	FILE *fp;
	char * opencl_program_src;
	long int size_cl_program=0;
	/*it returns the parsed char and as third arg it requires the separation char for arguments*/
	while ((option = getopt(argc,argv,"n:d:"))!=-1) {

		switch(option){

			case 'n':
				sscanf(optarg,"%d",&required_iterations);		
				break;

			case 'd':
				sscanf(optarg,"%f",&diffusion_const);
				break;

			default:
				fprintf(stdout,"no valid arguments\n");
				exit(-1);

		}
	}

	avg=0;
	fp=fopen(INPUT_FILE,"r");
	if(fp==NULL) {
		fprintf(stderr,"error openign the file \n");
		exit(-1);	
	}
	/*saving input files*/
	/*adding two additional rows and colums for avoididn to use control on the indexes*/
	fscanf(fp,"%d %d\n",&h,&w);
	size_t MATRIX_SIZE_N_PAD=h*w;
	h+=2;
	w+=2;
#ifndef ARRAY_MATRIX
	matrix=(float **) malloc ( sizeof(float  *) *h);
	if(matrix==NULL) {
		fprintf(stderr,"error allocating matrix\n");
		exit(-1);
	}

	for(int i=0;i<h;i++) {
		matrix[i]=(float *) malloc (sizeof(float) * (w));
		if(matrix[i]==NULL)  {
			fprintf(stderr,"errorr allocating row\n");
			exit(-1);

		}
		/*obtaining a clean matrix and with boundaries at zero*/
		for(int j=0;j<w;j++) {
			matrix[i][j]=0;

		}



	}
	/*populating matrix reading from file*/
	while(fscanf(fp,"%d %d %f\n",&x,&y,&val)!=EOF) {
		matrix[x+1][y+1]=val;

	}
#else
	/*calloc allows us to obtain a  clean matrix ( zeros) also with boundaries*/	
	matrix=(float *) calloc(h*w*2,sizeof(float));
	if(matrix==NULL) {
		fprintf(stderr,"eerror allocatin the matrix\n");
		exit(-1);
	}
	while(fscanf(fp,"%d %d %f\n",&x,&y,&val)!=EOF) {
		matrix [ (x+1)*w +(y+1) ]=val;
	}


#endif
	fclose(fp);
	if(fp==NULL) {
		fprintf(stderr,"error closing the file\n");
		exit(-1);
	}
#if DEBUG
printf("size %d %d\n",h,w);
	for(int i=0;i<h;i++) {
		for(int j=0;j<w;j++) {
#ifndef ARRAY_MATRIX
			fprintf(stdout,"%.3f ",matrix[i][j]);
#else
			fprintf(stdout,"%.3f ",matrix[ i*w +j]);
#endif
		}
		fprintf(stdout,"\n");
	}
#endif

float hl,hr,ha,hb,hh;
hl=hr=ha=hb=hh=0;
		/*computing heat diffusion */
for(size_t step=0;step<required_iterations ;step++) {

		for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
		hl=matrix [(step%2)*w*h+i*w+j-1];
		hr=matrix[(step%2)*w*h + i*w+j-1];
		ha=matrix[(step%2)*w*h+(i-1)*w+j];
		hb=matrix[(step%2)*w*h+(i+1)*w+j];
		hh=matrix[(step%2)*w*h+i*w+j];
		matrix[((step+1)%2)*w*h+i*w+j]=hh+diffusion_const*((ha+hb+hl+hr)/4-hh);
		}
		}


}

#if DEBUG
			for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
		fprintf(stdout,"%d ",matrix[i*w+j]);	
		}
		fprintf(stdout,"\n");
		}

#endif
	
/*compute average*/
avg=0;
		for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
		avg+=matrix[i*w+j];		
		}
		}
printf("here\n");
avg/=(MATRIX_SIZE_N_PAD);
fprintf(stdout,"Average temperature : %.2f\n",avg);

			/*compute abs */

		for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
		matrix[i*h+j]=fabs(matrix[i*w+j]-avg);		
		}
		}
			/*compute average*/
avg=0;
		for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
		avg+=matrix[i*w+j];		
		}
		}
avg/=(MATRIX_SIZE_N_PAD);
fprintf(stdout,"Average temperature : %.2f\n",avg);
#ifndef ARRAY_MATRIX 
	for(int i=0;i<h;i++) {
		free(matrix[i]);
	}
#endif
	free(matrix);
	return 0;
}


