#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 30000


int main (void){


	double a_re[N] , a_im[N] , b_re[N] ,b_im[N],c_re[N], c_im[N] ;
	struct timespec ts_start,ts_end;
	double time_ms=0;

	for(int  i=0; i< N ;i ++){
		b_re[i]=2.33;
		b_im[i]=4.88;
		c_re[i]=12.35;
		c_im[i]=35.813;
		a_re[i]=0;
		a_im[i]=0;

	}
	for( int i =0;i<N;i++) {


		if(!timespec_get(&ts_start,TIME_UTC)) {
			fprintf(stderr,"error during the acquisition of start time \n");
			exit(-1);
		}
		// mul function 
		a_re[i]= ( b_re[i]  *c_re[i] - c_im[i] *b_im[i] );
		a_im[i]= ( c_im[i]* b_re[i]+ b_im[i]*c_re[i]  ) ;
		if(!timespec_get(&ts_end,TIME_UTC)){
			fprintf(stderr,"error during the acquisition of end time \n");
			exit(-1);
		}

		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


	}

	fprintf(stdout,"########################################\n#### Execution time: %f [ms]  ####\n########################################\n", time_ms/N  );


	return 0;
}



