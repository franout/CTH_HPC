#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define INPUT_FILE "./try.txt"
#include <mpi.h>

#define ARRAY_MATRIX 1
#define DEBUG 0

typedef enum {COMPUTE,AVG,DIFF_ABS,DONE_COMPUTATION}cmd_t; 


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

	/*initialization */
	MPI_Init(&argc, &argv);
	cmd_t cmd;
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


	int nmb_mpi_proc, mpi_rank;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &nmb_mpi_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

#if DEBUG
	printf( "Number of processes: %d\n", nmb_mpi_proc );
#endif
	/*heat diffusion */
	if (mpi_rank==0) {
		/* master process */
		/*read input data*/
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
		matrix=(float *) calloc(h*w,sizeof(float));
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
		/*basic pattern send receiver*/
		/*notify to workers that thye can proceed with the computation*/

		for( size_t step=0;step<required_iterations;step++) {
			cmd=COMPUTE;
			MPI_Send(&cmd, 1, MPI_INT, 1, step, MPI_COMM_WORLD);

			while ( cmd!=DONE_COMPUTATION) {
			MPI_Recv(&cmd,1,MPI_INT,1,step,MPI_COMM_WORLD,&status);
			}

		}
	
#if DEBUG
	printf("%d %d size\nComputed matrix",h,w);	
	for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
#ifndef ARRAY_MATRIX 
			fprintf(stdout,"%.2f ",matrix[i][j]);
#else
			fprintf(stdout,"%.2f ", matrix[ i*w + j] );
#endif

		}
		fprintf(stdout,"\n");

	}

#endif



		/*compute average*/

		/*compute abs diff */

		/*compute average*/


	}

	else if (mpi_rank==1) {
		/*workers */
		int step_work=0;
		while(1) {
			MPI_Recv(&cmd, 1, MPI_INT,0 , step_work, MPI_COMM_WORLD, &status);

			switch(cmd) {
				case COMPUTE:

					for(int ix=1;ix<h-1;ix++) {
						for (int jy=1;jy<w-1;jy++) {
							float hl=matrix[ix*w + jy-1] ; //h(i,j-1)
							float hr=matrix[ix*w+jy+1] ;  // h(i,j+1)
							float ha= matrix[(ix-1)*w+jy]; //h(i-1,j)
							float hb=matrix[(ix+1)*w+jy] ; // h(i+1,j)
									float h=matrix[ix*w + jy];



									float local_t=h+diffusion_const*((ha+hb+hl+hr)/4-h);

									matrix[ix*w+jy]=local_t;


									}


									}
									MPI_Send((cmd_t *)DONE_COMPUTATION,1,MPI_INT,0,step_work,MPI_COMM_WORLD);		
									step_work++;

									break;
									case AVG:
									break;
									case DIFF_ABS:

									break;

									default:
									fprintf(stderr,"unrecognized command\n");
									exit(-1);



			}
		}
	}


	/*finishing */ 

	MPI_Finalize();

	avg=0;





#if DEBUG 
	fprintf(stdout,"calculating average\n");
#endif	





#if DEBUG
	avg=0;
	for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
#ifndef ARRAY_MATRIX
			avg+=matrix[i][j];	
#else
			avg+=matrix[i*w+j];
#endif
		}

	}
	avg/=(MATRIX_SIZE_N_PAD);
	fprintf(stdout,"Average temperature (debug check): %.2f\n",avg);
	fprintf(stdout,"calculating abs difference with previous avg kernel \n");
#endif

#if DEBUG
	avg=111080;
	for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
#ifndef ARRAY_MATRIX
			fprintf(stdout,"%.3f ",fabs(matrix[i][j]-avg));

			fprintf(stdout,"%.3f ",matrix[i][j]);
#else 
			fprintf(stdout,"(debug i.e. normal value) %.3f vs %.3f ",fabs(matrix[i*w + j]-avg),matrix[i*w+j]);

#endif

		}
		fprintf(stdout,"\n");


	}

#endif


	/*computing again the average on the absolute values*/

#if DEBUG
	avg=111081110811;
#endif

#if DEBUG
	avg=0;
	for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
#ifndef ARRAY_MATRIX
			avg+=matrix[i][j];	
#else
			avg+=matrix[i*w+j];
#endif
		}

	}
	avg/=(MATRIX_SIZE_N_PAD);
	fprintf(stdout,"Average temperature of absolute value(debug): %.2f\n",avg);

#endif








#ifndef ARRAY_MATRIX 
	for(int i=0;i<h;i++) {
		free(matrix[i]);
	}
#endif
	free(matrix);
	return 0;
}


