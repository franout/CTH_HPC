#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <stddef.h>
/*prototype of the threads*/
static void * computation_task(void * args);
static void * writing_task(void * args);

/*static global  variables */
static int N_THREAD,n_row_col, degree;
static struct timespec sleep_timespec;
/*mutex*/
static pthread_mutex_t item_done_mutex;


/*variables for data transfer*/
int ** results;
char * item_done;


int main (int argc, char ** argv ) {

pthread_t *threads_computation,thread_WR;
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

results=(int **) malloc(sizeof(int *) * n_row_col);
if(results==NULL) {
fprintf(stderr,"error allocating global variable for data transfer\n");
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
results[i]=NULL;
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



/*creating writing thread*/
 if ((ret = pthread_create(&thread_WR, NULL, writing_task,(void *)0 ))) {
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


free(results);
free(item_done);
free(threads_computation);





return 0;
}



static void * writing_task ( void * args ) {
  char * item_done_loc = (char*)calloc(n_row_col, sizeof(char));


  for ( size_t ix = 0; ix < n_row_col; ) {
	    pthread_mutex_lock(&item_done_mutex);
	      if ( item_done[ix] != 0 )
		          memcpy(item_done_loc, item_done, n_row_col*sizeof(char));
	        pthread_mutex_unlock(&item_done_mutex);

		  if ( item_done_loc[ix] == 0 ) {
			     nanosleep(&sleep_timespec, NULL);
			          continue;
				    }
		int * result = (int * ) malloc ( sizeof(int) * n_row_col);
		    for ( ; ix < n_row_col && item_done_loc[ix] != 0; ++ix ) {
			       if(results[ix]!=NULL){
			   	 result = results[ix];
				} else {
				fprintf(stderr,"something went wrong with the assignment of results \n");
				exit(-1);
				}
				    //TODO: write result
				   fprintf(stdout,"%d",ix); 
				        free(result);
				          }
				          }

  free(item_done_loc);
return NULL;
}


static void * computation_task(void * args ) {
	size_t offset=*((size_t *)args);
	free(args);


	  for (size_t ix = offset; ix < n_row_col; ix += N_THREAD ) {
		    int * result = (int*)malloc(sizeof(int)*n_row_col);
		      //TODO :compute work item checking correctness
		        results[ix] = result;
		       nanosleep(&sleep_timespec, NULL);
		          pthread_mutex_lock(&item_done_mutex);
		            item_done[ix] = 1;
		              pthread_mutex_unlock(&item_done_mutex);
		              }

	return NULL;
}
