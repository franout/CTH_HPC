#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>


/*prototype of the threads*/
static void * computation_task(void * args);
static void * writing_task(void * args);

/*static global  variables */
static int N_THREAD,n_row_col, degree;
volatile static int finish_computation;
typedef struct {
char *convergence, *attractors;
} args_wr;

int main (int argc, char ** argv ) {
args_wr args_wr_task;
pthread_t *threads_computation,thread_WR;
int i,ret;
int option=0;
int pipe[2]; // for communicating between computation threads and writing thread
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
finish_computation=1;
threads_computation=(pthread_t *) malloc(sizeof(pthread_t)*N_THREAD);
if(threads_computation==NULL){
fprintf(stderr,"error allocating threads' array\n");
exit(-1);
}


/*creating computation thread*/
for ( i =0;i< N_THREAD;i++) {
	
 if ((ret = pthread_create(&(threads_computation[i]), NULL,  computation_task, (int * )0 ))) {
	     fprintf(stderr,"Error %d creating thread: %d \n", ret,i);
	         exit(1);
		   }
}
args_wr_task.convergence=(char *) malloc ( sizeof(char) *30 );
args_wr_task.attractors= (char * ) malloc (sizeof(char) *30);

if(args_wr_task.convergence == NULL || args_wr_task.attractors==NULL) {
fprintf(stderr,"error allocating string's path of fd\n");
exit(-1);
}


sprintf(args_wr_task.convergence, "./newton_convergence_x%d.ppm",degree);
sprintf(args_wr_task.attractors, "./newton_attractors_x%d.ppm",degree);
/*creating writing thread*/
 if ((ret = pthread_create(&thread_WR, NULL, writing_task, (void*)(&args_wr_task)))) {
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

free(threads_computation);
free ( args_wr_task.convergence);
free(args_wr_task.attractors);


return 0;
}



static void * writing_task ( void * args ) {

FILE * att, * conv;
args_wr * p_args;
char * work_string;
p_args= args;


if((att=fopen(p_args->attractors,"w"))){
fprintf(stderr,"erorr opening file attractors\n");
exit(-1);
}
if((conv=fopen(p_args->convergence,"w"))){
fprintf(stderr,"erorr opening file attractors\n");
exit(-1);
}
work_string= (char *) malloc ( sizeof(char )*128);
if(work_string==NULL){
fprintf(stderr,"error allocating string\n");
exit(-1);

}
sprintf(work_string,"P3\n%d %d\n255\n",n_row_col,n_row_col);
fwrite(work_string, sizeof(char), strlen(work_string) ,att);
fwrite(work_string,sizeof(char),strlen(work_string),conv);


while(finish_computation){

	
/*reading from the pipe*/		
if(read()){
fprintf(stderr,"error reading from the pipe\n");
exit(-2);
}	


fwrite(work_string,sizeof(char),strlen(work_string),conv);
fwrite(work_string,sizeof(char),strlen(work_string),conv);
}


fclose(att);
fclose(conv);
if(att==NULL || conv==NULL){
fprintf(stderr,"error closing files \n");
exit(-1);}

free(work_string);

return NULL;
}


static void * computation_task(void * args ) {
for ( conv = 0, attr = DEFAULT_VALUE; ; ++conv ) {
  if ( CHECK CONDITION ) {
      attr = VALUE;
          break;
            }
              if ( CHECK CONDITION ) {
                  attr = VALUE;
       	       break;
       	         }
       		   for ( EXPRESSION )
       		       if ( CHECK CONDITION ) {
       		             attr = VALUE_NOT_EQUAL_TO_THE_DEFAULT_ONE;
       			           break;
   }
     if ( attr != DEFAULT_VALUE )
         break;

           // computation
     }
       						       


	finish_computation=0;
	return NULL;
}
