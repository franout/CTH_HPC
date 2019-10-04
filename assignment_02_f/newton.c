#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <complex.h>
/*Implement in C using POSIX threads a program called newton that computes similar pictures
 *  for a given functions f(x) = x^d - 1 for complex numbers with real and imaginary part between -2 and +2.
*/ 

/*prototype of the threads*/
static void * computation_task(void * args);
static void * writing_task(void * args);

/*static global  variables */
static int N_THREAD,n_row_col, degree;
static struct timespec sleep_timespec;
/*mutex*/
static pthread_mutex_t item_done_mutex;


/*variables for data transfer*/
static char * item_done;
static uint8_t ** attractors; // roots in which the function is evolving
static uint8_t ** convergences; // # of iterations needed for the convergence to a root
/*struct for passing argument to write threa*/

typedef struct {
char *attractors_file, *convergences_file;
}write_args;

int main (int argc, char ** argv ) {


pthread_t *threads_computation,thread_WR;
write_args write_t_args ;
int i,ret;
int option=0;

/*it returns the parsed char and as third arg it requires the separation char for arguments*/
while ((option = getopt(argc,argv,"t:l:"))!=-1) {

	switch(option){
	
		case 't':
		sscanf(optarg,"%d",&N_THREAD);		
			break;

		case 'l':
			sscanf(optarg,"%d",&n_row_col);
			break;
		default:
			fprintf(stdout,"no valid arguments\n");
			exit(-1);
	
	}
}


sscanf(argv[argc-1],"%d",&degree);
sleep_timespec.tv_nsec=100000;
threads_computation=(pthread_t *) malloc(sizeof(pthread_t)*N_THREAD);
if(threads_computation==NULL){
fprintf(stderr,"error allocating threads' array\n");
exit(-1);
}
attractors=(double complex **) malloc(sizeof(double complex *)*n_row_col);
if(attractors==NULL){
fprintf(stderr,"error allocating attractor vector pointer\n");
exit(-1);
}
convergences=(double complex**)malloc(sizeof(double complex *)*n_row_col);
if(convergences==NULL){
fprintf(stderr,"error allocating convergence vector pointer\n");
exit(-1);
}

item_done=(char *) malloc(sizeof(char)*n_row_col);
if(item_done==NULL) {
fprintf(stderr,"error  allocating global variable for data transfer\n");
exit(-1);
}
// initializing to all zero  item done and to null the pointer of results
for(i=0;i<n_row_col;i++){
item_done[0]=0;
}


/*creating computation thread*/
for ( i =0;i< N_THREAD;i++) {
size_t *args = malloc(sizeof(size_t));
if (args==NULL){
fprintf(stderr,"error allocating arguments for computation threads\n");
exit(-1);
}
*args=i;
 if ((ret = pthread_create(&(threads_computation[i]), NULL,  computation_task, (void * )args ))) {
	     fprintf(stderr,"Error %d creating thread: %d \n", ret,i);
	         exit(1);
		   }
}

write_t_args.attractors_file=(char *) malloc ( sizeof(char) *30);
 write_t_args.convergences_file=(char *) malloc(sizeof(char)*30);
if(write_t_args.convergences_file==NULL || write_t_args.attractors_file==NULL){
fprintf(stderr,"error allocating string for file name \n");
exit(-1);
}
sprintf(write_t_args.convergences_file,"newton_convergence_x%d.ppm",degree);
sprintf(write_t_args.attractors_file,"newton_attractors_x%d.ppm",degree);

/*creating writing thread*/
 if ((ret = pthread_create(&thread_WR, NULL, writing_task,(void *)(&write_t_args) ))) {
	     fprintf(stderr,"Error %d creating thread: %d \n", ret,i);
	         exit(1);
		   }


/*waiting for all threads*/


for (i=0;i<N_THREAD;i++) {
	  if ((ret = pthread_join(threads_computation[i], NULL))) {
		fprintf(stderr,"Error %d creating thread: %d \n", ret,i);
     		          exit(1);
			    }
}
/*waiting the writing thread if it has still work to do */
if ((ret = pthread_join(thread_WR, NULL))) {
		fprintf(stderr,"Error %d creating thread: %d \n", ret,i);
     		          exit(1);
			    }


free(item_done);
free(threads_computation);
free(convergences);
free(attractors);
return 0;
}



static void * writing_task ( void * args ) {
 	 char * item_done_loc = (char*)calloc(n_row_col, sizeof(char));
	  char*work_string;

	/*they are just poitners to the row which have to write*/
	uint8_t* result_c ;
	uint8_t* result_a;


	FILE * fp_attr,*fp_conv;
	write_args * files_local=(write_args *) args;
	fp_attr= fopen(files_local->attractors_file,"w");
	if(fp_attr==NULL){
	fprintf(stderr,"errror opening attractors file\n");
	exit(-1);
	}
	fp_conv=fopen(files_local->convergences_file,"w");
	if(fp_conv==NULL){
	fprintf(stderr,"error opening convergences file\n");
	exit(-1);
	}

	free(files_local->convergences_file);
	free(files_local->attractors_file);
	// depending on the degree we will have d roots plus two special roots ( 0 and inf )
work_string=(char *) malloc ( sizeof(char)* 30);
if(work_string==NULL){
fprintf(stderr,"error allocating working string\n");
exit(-1);
}
sprintf(work_string,"P3\n%d %d\n%d\n",n_row_col,n_row_col,degree+2);
fwrite(work_string,sizeof(char),strlen(work_string),fp_attr);
sprintf(work_string,"P3\n%d %d\n%d\n",n_row_col,n_row_col,2);
fwrite(work_string,sizeof(char),strlen(work_string),fp_conv);
	for ( size_t ix = 0; ix < n_row_col; ) {
	    pthread_mutex_lock(&item_done_mutex);
	      if ( item_done[ix] != 0 )
		          memcpy(item_done_loc, item_done, n_row_col*sizeof(char));
	        pthread_mutex_unlock(&item_done_mutex);

		  if ( item_done_loc[ix] == 0 ) {
			     nanosleep(&sleep_timespec, NULL);
			          continue;
				    }
		    for ( ; ix < n_row_col && item_done_loc[ix] != 0; ++ix ) {
				result_c=convergences[ix];
				result_a=attractors[ix];
				for(i=0;i<n_row_col;i++) {
				sprintf(work_string,"%d ",result_a[i]);
				fwrite(work_string,sizeof(char),strlen(work_string),fp_attr);	
				sprintf(work_string,"%d ",result_c[i]);
				fwrite(work_string,sizeof(char),strlen(work_string),fp_conv);
								
				}
						
				 				          }
				          }
  free(work_string);
  free(item_done_loc);
  fclose(fp_attr);
  fclose(fp_conv);
  if(fp_attr==NULL || fp_conv==NULL){
  fprintf(stderr,"errror closing output files\n");
  exit(-1);
  }


return NULL;
}


static void * computation_task(void * args ) {
	size_t offset=*((size_t *)args);
	free(args);
	double complex x,y,z;
	double complex attr;
	int conv; //CHECK TODO
  for (size_t ix = offset; ix <n_row_col; ix += N_THREAD ) {
     		uint8_t * attractor=(uint8_t *) malloc(sizeof(uint8_t) *n_row_col);
		uint8_t  complex * convergence=(uint8_t*) malloc(sizeof(uint8_t) *n_row_col);
		if( attractor==NULL || convergence==NULL) {
		fprintf(stderr,"error allocating rows in the computation thread\n");
		exit(-1);	
		}
	//TODO choose a starting point 

	// for the column along x axes
	for(size_t jx=0 ; ; jx++ ){

    //TODO :compute work item  and checking correctness
	for ( conv = 0, attr =1; ; ++conv ) {
	  if ( CHECK CONDITION ) {
		      attr = VALUE;
		          break;
			    }
	    if ( CHECK CONDITION ) {
		        attr = VALUE;
			    break;
			      }
	      for ( EXPRESSION ){
		          if ( CHECK CONDITION ) {
				        attr = VALUE_NOT_EQUAL_TO_THE_DEFAULT_ONE;
					      break;
					          }	}
	        if ( attr != DEFAULT_VALUE ){
			    break;
		}
		// computing x_k+1
		   y=x; // getting the current x
		   z=x;
			for(int j=1;j<degree;j++) {
			y=cmul(x,y);	
			z=cmul(x,z);
			}
			y=cmul(y,x);
			y--;	
			z=cmul(++j,z); // multipling by d
			// y has x_k^d z has x_k^(d-1)*d
			x=x - cdiv(y,z); 
			// attractors are the imaginary part of the complex number
				
		              }//TODO
			attractor[jx]=attr; // maping function for color
			convergence[jx]=conv; // mapping function for tocolo
		}
			nanosleep(&sleep_timespec, NULL);
		   	 pthread_mutex_lock(&item_done_mutex);
			  attractors[ix]=attractor;
		       convergences[ix]=convergence;
		           item_done[ix] = 1;
		             pthread_mutex_unlock(&item_done_mutex);  
	
  }
	return NULL;
}
