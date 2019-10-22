#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#define NUM_IT 15
#define N 1000000000

int main (void ) {

long int sum;
struct timespec ts_start,ts_end;
double time_ms=0;
	
for(int j =0;j<NUM_IT ;j++){
sum=0;

if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of start time!\n");
	exit(-1);
}

// computing the sum
for( int  i=1; i<=N ;i++){

sum += i;

}

if(!timespec_get(&ts_end, TIME_UTC)){
	fprintf(stderr,"erorr during the acquisition of end time!\n");
	exit(-1);
}
time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


}



fprintf(stdout,"naive computation of the sum of the first billion integers %ld\n",sum);
fprintf(stdout,"Execution time: %f [ms] \n", ((double)time_ms)/NUM_IT);
	





return 0;
}
