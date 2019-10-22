#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 30000

void mul_cpx_mainfile( double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im  ) ;



int main (void){
struct timespec ts_end,ts_start;
double a_re[N] , a_im[N] , b_re[N] ,b_im[N],c_re[N], c_im[N] ;
int i;
double time_ms=0;

for( i=0; i< N ;i ++){
	b_re[i]=2.33;
	b_im[i]=4.88;
	c_re[i]=12.35;
	c_im[i]=35.813;
	a_re[i]=a_im[i]=0;
}


for(i=0;i<N;i++) {
if(!timespec_get(&ts_start,TIME_UTC)) {
fprintf(stderr,"error during the acquisition of start time \n");
exit(-1);
}


mul_cpx_mainfile(a_re+i,a_im+i, b_re+i,b_im,c_re+i, c_im+i);


if(!timespec_get(&ts_end,TIME_UTC)){
fprintf(stderr,"error during the acquisition of end time \n");
exit(-1);
}

time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;
}

fprintf(stdout,"########################################\n#### Execution time: %f [ms]  ####\n########################################\n", ((double)time_ms)/N  );







return 0;
}


void mul_cpx_mainfile( double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im  ) {


*a_re= ( (*b_re ) *(*c_re) - (*c_im)*(*b_im) );
*a_im= ( (*c_im)*(* b_re)+(* c_re)*(*b_im) );


	return ;
}
