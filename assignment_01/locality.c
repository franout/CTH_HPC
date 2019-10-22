#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000
#define NUMB_IT 15


void col_sums_opt(   double * sums,  const double ** matrix,  size_t nrs,   size_t ncs  );
void col_sums(   double * sums,  const double ** matrix,  size_t nrs,   size_t ncs  );
void row_sums( double * sums,const double ** matrix, size_t nrs,   size_t ncs); 


int main (void){
 double ** matrix ;
double sum_r[N],sum_c[N], time_ms;
u_int16_t i , j;
struct timespec ts_end,ts_start;
// populating the matrix in row major order 

matrix=(double **) malloc ((sizeof(double *) * N));
if(matrix==NULL) {
fprintf(stderr,"erorr allocating matrix\n");
exit(-1);

}


for (i=0;i<N;i++){
	matrix[i]=(double *) malloc (sizeof(double) *N);
	if (matrix[i]==NULL){
	fprintf(stderr,"error allocating matrix \n");
	exit(-1);	
	}


	for(j=0;j<N;j++){
	matrix[i][j]=1.01;
	}
	// initializing vectors sums to all zeros
	sum_r[i]=0;
	sum_c[i]=0;
}
time_ms=0;
// don't care of possible overflow of vector sum
for ( u_int8_t it=0; it <NUMB_IT; it++) {
if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of start time!\n");
	exit(-1);
}
col_sums(sum_c ,(const double ** )  matrix,N,N );
if(!timespec_get(&ts_end, TIME_UTC)){
	fprintf(stderr,"erorr during the acquisition of end time!\n");
	exit(-1);
}


time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;
}
fprintf(stdout,"Execution time of columns sum: %f [ms]\n",((double ) time_ms)/NUMB_IT );
time_ms =0;
for ( u_int8_t it=0; it <NUMB_IT; it++) {
if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of start time!\n");
	exit(-1);
}
row_sums(sum_r ,(const double ** )  matrix,N,N );
if(!timespec_get(&ts_end, TIME_UTC)){
	fprintf(stderr,"erorr during the acquisition of end time!\n");
	exit(-1);
}time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


}
fprintf(stdout,"Execution time of rows sum: %f [ms]\n",((double)time_ms)/NUMB_IT );



// since the matrix is stored in row major order for sure the column sum will be slow
time_ms=0;

for ( u_int8_t it=0; it <NUMB_IT; it++) {
if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of start time!\n");
	exit(-1);
}
col_sums_opt(sum_c ,(const double ** )  matrix,N,N );
if(!timespec_get(&ts_end, TIME_UTC)){
	fprintf(stderr,"erorr during the acquisition of end time!\n");
	exit(-1);
}
time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


}
fprintf(stdout,"Execution time of columns sum optimized: %f [ms]\n",((double) time_ms)/NUMB_IT );


for(i=0;i<N;i++) {

free(matrix[i]);

}
free(matrix);

return 0;
	}

void row_sums( double * sums,const double ** matrix, size_t nrs,   size_t ncs) 
{
	  for ( size_t ix=0; ix < nrs; ix++ ) {
		      double sum = 0;
		          for ( size_t jx=0; jx < ncs; jx++ )
				        sum += matrix[ix][jx];
			      sums[ix] = sum;
			        }
}

void col_sums(   double * sums,  const double ** matrix,  size_t nrs,   size_t ncs  )
{
	  for ( size_t jx=0; jx < ncs; jx++ ) {
		      double sum = 0;
		          for ( size_t ix=0; ix < nrs; ix++ )
				        sum += matrix[ix][jx];
			      sums[jx] = sum;
			        }
}


void col_sums_opt(   double * sums,  const double ** matrix,  size_t nrs,   size_t ncs  )
{
		// loop unrolling for avoiding page miss and improving the data loaded into the data cache 
double sum =0; // temporary value, it may be a vector of length equal to the # of unroll of the loop
		for ( size_t jx=0; jx < ncs/10; jx++ ) {
			sum =0;	     
		             for (   size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx];
					sum += matrix [ ix+1][jx];
					sum += matrix [ ix+2][jx];
					sum += matrix [ ix+3][jx];
					sum += matrix [ ix+4][jx];

		      sums[jx] = sum;
			        }
			     sum=0;
	  			for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+1];		
					sum += matrix [ ix+1][jx+1];
					sum += matrix [ ix+2][jx+1];
					sum += matrix [ ix+3][jx+1];
					sum += matrix [ ix+4][jx+1];


			      sums[jx+1] = sum;
			        }
				sum =0;
				for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+2];
					sum += matrix [ ix+1][jx+2];
					sum += matrix [ ix+2][jx+2];
					sum += matrix [ ix+3][jx+2];
					sum += matrix [ ix+4][jx+2];


			      sums[jx+2] = sum;
			        }
				sum=0;
				for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+3]; 
				sum += matrix [ ix+1][jx+3];
					sum += matrix [ ix+2][jx+3];
					sum += matrix [ ix+3][jx+3];
					sum += matrix [ ix+4][jx+3];


			      sums[jx+3] = sum;
			        }
				sum=0;
				for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+4];
					sum += matrix [ ix+1][jx+4 ];
					sum += matrix [ ix+2][jx+4];
					sum += matrix [ ix+3][jx+4];
					sum += matrix [ ix+4][jx+4];

		      sums[jx+4] = sum;
			        }

				sum =0;	     
		             for (   size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+5];
						sum += matrix [ ix+1][jx+5];
					sum += matrix [ ix+2][jx+5];
					sum += matrix [ ix+3][jx+5];
					sum += matrix [ ix+4][jx+5];

	      sums[jx+5] = sum;
			        }
			     sum=0;
	  			for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+6];
							sum += matrix [ ix+1][jx+6];
					sum += matrix [ ix+2][jx+6];
					sum += matrix [ ix+3][jx+6];
					sum += matrix [ ix+4][jx+6];

      sums[jx+6] = sum;
			        }
				sum =0;
				for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+7];
			     				sum += matrix [ ix+1][jx+7];
					sum += matrix [ ix+2][jx+7];
					sum += matrix [ ix+3][jx+7];
					sum += matrix [ ix+4][jx+7];

				 sums[jx+7] = sum;
			        }
				sum=0;
				for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+8];
			     				sum += matrix [ ix+1][jx+8];
					sum += matrix [ ix+2][jx+8];
					sum += matrix [ ix+3][jx+8];
					sum += matrix [ ix+4][jx+8];

				 sums[jx+8] = sum;
			        }
				sum=0;
				for (  size_t ix=0; ix < nrs/5; ix++ ) {
				        sum += matrix[ix][jx+9];
			      		sum += matrix [ ix+1][jx+9];
					sum += matrix [ ix+2][jx+9];
					sum += matrix [ ix+3][jx+9];
					sum += matrix [ ix+4][jx+9];

					sums[jx+9] = sum;
			        }
		        }

	
}
