#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#define MAX_IT 50
#define BUFFER_SIZE 4096
/*Implement in C using POSIX threads a program called newton that computes similar pictures
 *  for a given functions f(x) = x^d - 1 for complex numbers with real and imaginary part between -2 and +2.
 */ 

/*prototype of the threads*/
static void * computation_task(void * args);
static void * writing_task(void * args);

/*stati global  variables */
static int N_THREAD,n_row_col, degree;
static double step;
static struct timespec sleep_timespec;

typedef struct {
	double * angles;
	size_t n;
} roots;

static  roots LUT;
const float TPI=6.293;
/*mutex*/
static pthread_mutex_t item_done_mutex;
/*variables for data transfer*/
static char * item_done;
static int ** attractors; // roots in which the function is evolving
static u_int8_t**  convergences; // # of iterations needed for the convergence to a root

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
	step=4/((double)n_row_col);
	sleep_timespec.tv_nsec=100000;
	threads_computation=(pthread_t *) malloc(sizeof(pthread_t)*N_THREAD);	if(threads_computation==NULL){
		fprintf(stderr,"error allocating threads' array\n");
		exit(-1);
	}
	attractors=(int   **) malloc(sizeof( int *)*n_row_col);
	if(attractors==NULL){
		fprintf(stderr,"error allocating attractor vector pointer\n");
		exit(-1);
	}
	convergences=(u_int8_t **)malloc(sizeof(u_int8_t *)*n_row_col);
	if(convergences==NULL){
		fprintf(stderr,"error allocating convergence vector pointer\n");
		exit(-1);
	}
	item_done=(char *) calloc(n_row_col,sizeof(char));
	if(item_done==NULL) {
		fprintf(stderr,"error  allocating global variable for data transfer\n");
		exit(-1);
	}
	LUT.n=degree+2;
	LUT.angles=(double*) malloc(sizeof(double) *(LUT.n));
	if(LUT.angles==NULL) {
		fprintf(stderr,"error allocating the LUT\n");
		exit(-1);

	}
	for(i=0;i<degree;i++){
		LUT.angles[i]=((double)TPI)/degree*i;
	}
	LUT.angles[LUT.n-2]=999.00; // value for 0
	LUT.angles[LUT.n -1]=888.00; // value for ing



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
	sprintf(write_t_args.convergences_file,"./newton_convergence_x%d.ppm",degree);
	sprintf(write_t_args.attractors_file,"./newton_attractors_x%d.ppm",degree);

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
	free(LUT.angles);
	return 0;
}



static void * writing_task ( void * args ) {
	char * item_done_loc = (char*)calloc(n_row_col, sizeof(char));
	char*work_string,*work_string_attr;

	/*they are just poitners to the row which have to write*/
	u_int8_t * result_c;
	int  * result_a;
	char ** grey_scale;
	char **rgb_scale;
	double  mt =255.0/(MAX_IT);
	int i,old_i,offset_str;
	size_t j=0;
	double sum ;
	FILE * fp_attr, *fp_conv;
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
	grey_scale=(char **) malloc ( sizeof(char*)*(MAX_IT+1));
	if(grey_scale==NULL) {
		fprintf(stderr,"error allocating grey scale\b");
		exit(-1);
	}
	sum=0;
	//compute grey scale
	for(j=0;j<MAX_IT+1;j++) {
		grey_scale[j]=(char *)malloc(sizeof(char)*12);
		sprintf(grey_scale[j],"%d %d %d " , (int)sum,(int) sum , (int)sum);
		sum+=mt;
		//printf("%s\n",grey_scale[j]);
	}
	rgb_scale=(char **) malloc(sizeof(char  *)*(degree+2));
	if(rgb_scale==NULL) {
		fprintf(stderr,"error allocating rgb scale\n");
		exit(-1);	
	}
//	printf("\n\n");
	/*compute rgb matrixt*/
	mt=255.0/((degree+2));
	sum = 0;
	for(j=0;j<degree+2;j++) {
		rgb_scale[j]=(char *) malloc(sizeof(char)*12);
		sprintf(rgb_scale[j],"%d %d %d ", (int)(255.00-sum),(int)sum,(int)( 100 + sum)%100);
		sum+=mt;
	//	printf("%s\n",rgb_scale[j]);	
	}
	free(files_local->convergences_file);
	free(files_local->attractors_file);
	// depending on the degree we will have d roots plus two special roots ( 0 and inf )
	work_string=(char *) malloc ( sizeof(char)* BUFFER_SIZE);
	work_string_attr=(char *) malloc ( sizeof(char)* BUFFER_SIZE);
	if(work_string==NULL || work_string_attr==NULL){
		fprintf(stderr,"error allocating working string\n");
		exit(-1);
	}

	// header of the files 
	sprintf(work_string,"P3\n%d %d\n%d\n",n_row_col,n_row_col,degree+2);
	fwrite(work_string,sizeof(char),strlen(work_string),fp_attr);	
	sprintf(work_string,"P3\n%d %d\n255\n",n_row_col, n_row_col);
	fwrite(work_string,sizeof(char),strlen(work_string),fp_conv);
	for ( size_t ix = 0; ix < n_row_col; ) {
		//revisit loop structure for performance improvements later
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
			for(old_i=0;old_i<n_row_col; ) {
				offset_str=0;
				work_string[0]='\0';
				work_string_attr[0]='\0';
				for( i=old_i; i<n_row_col && offset_str+12+1<BUFFER_SIZE;i++) {
					memcpy(work_string_attr+offset_str,rgb_scale[result_a[i]] ,12);
					memcpy(work_string+offset_str,grey_scale[result_c[i]], 12);	
					offset_str+=12;	
				}		
				old_i=i;
				memcpy(work_string_attr+offset_str, "\n", 1);
			        memcpy(work_string+offset_str, "\n", 1);	
				fwrite(work_string_attr,sizeof(char),offset_str,fp_attr);
				fwrite(work_string,sizeof(char),offset_str,fp_conv);

			}

			free(result_a);
			free(result_c);
		}
	}
	free(work_string);
	free(work_string_attr);
	free(item_done_loc);
	for(j=0;j<MAX_IT+1;j++) {
		free(	grey_scale[j]);	}
	free(grey_scale); 
	for(j=0;j<degree+2;j++) {
		free(rgb_scale[j]);
	}
	free(rgb_scale);
	fclose(fp_conv);
	fclose(fp_attr);
	if(fp_attr==NULL || fp_conv==NULL){
		fprintf(stderr,"errror closing output files\n");
		exit(-1);
	}


	return NULL;
}


static void * computation_task(void * args ) {
	size_t offset=*((size_t *)args);
	free(args);
	u_int8_t conv;
	double complex x,y;
	double  attr,x_re,x_im,mod,asc_step,phase,old_x;
	int j,k;
	const double step_local=step;
	const double div=1.00/degree;
	// for the row along y axe
	for (size_t ix = offset; ix <n_row_col; ix += N_THREAD ) {
		int  * attractor=(int *) malloc(sizeof(int ) *n_row_col);
		u_int8_t  * convergence=(u_int8_t* ) malloc(sizeof(u_int8_t) *n_row_col);
		if( attractor==NULL || convergence==NULL) {
			fprintf(stderr,"error allocating rows in the computation thread\n");
			exit(-1);	
		}
		asc_step=ix*step_local;
		// for the column along x axe
		for(size_t jx=0 ;jx<n_row_col; jx++ ){
			x=(-2+jx*step_local)+I*(2-asc_step);  // initial point

			for ( conv = 0, attr =0;conv<MAX_IT ; ++conv ) {
			//	mod=cabs(x);
				x_re=creal(x);
				x_im=cimag(x);
				mod = x_re*x_re +x_im*x_im;
				if ( mod<= 1e-3){ // converging to zero
					attr = LUT.n-2; 
					break;
				}
				if ( x_re>=1000000000L || x_re<=-1000000000L ||x_im >=10000000000L || x_im<=-10000000000L ) { // convergin o inf

					attr=LUT.n-1;
					break;
				}
				if(mod<=1+1e-3){
					phase=fabs(carg(x));
					for (k=0; k<LUT.n-2 ;k++ ){
						if (   LUT.angles[k]-phase<=1e-3 ||   LUT.angles[k]-phase>=1e-3 ) {
							attr=k;
							break;
						}

					}

					if(attr!=0) {
						break;
					}	
				}

				// computing x_k+1
				y=1;
				for(j=0;j<degree;j++) {
					y*=x;				

				}
				old_x=creal(x);
				old_x*=old_x;
				y=1.00/y;
				x_im=cimag(x);
				x_im*=x_im;
				old_x=old_x+x_im;	
				x=x*(1+0*I-div*(1+0*I-y));
				if ( mod - old_x<=1e-10) {
					attr=LUT.n-2;	
					break;
				}

			}
			// find a possible root
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

