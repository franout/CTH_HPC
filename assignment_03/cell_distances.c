#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#define INPUT_FILE "./cell_e4"
#define ROW_L 24// length of row  + \n
#define BUFFER_SIZE  24000



static int *occ;
static float ** point;


int main (int argc , char ** argv ) {

	int N_THREAD=0;
	int option=0;
	int	dist,points,n;
	char* work_string,*tmps; 
	FILE * fp;
	int read_char;
	/*it returns the parsed char and as third arg it requires the separation char for arguments*/
	while ((option = getopt(argc,argv,"t:"))!=-1) {

		switch(option){

			case 't':
				sscanf(optarg,"%d",&N_THREAD);
				break;
			default:
				fprintf(stdout,"no valid arguments\n");
				exit(-1);
		}
	}
	omp_set_num_threads(N_THREAD);
	work_string=(char *)calloc(BUFFER_SIZE+1, sizeof(char));

//#pragma omp declare reduction (int_occ : int  : map.occ[omp_in]++ )



	tmps=(char *) calloc(ROW_L+1,sizeof(char));
	if(work_string==NULL || tmps==NULL) {
		fprintf(stderr,"error allocating strings\n");
		exit(-1);
	}
	fp=fopen(INPUT_FILE,"r");
	if(fp==NULL ) {
		fprintf(stderr,"errror opening the file\n");
		exit(-1);
	}
	n=3465+1; // fp *100 -> max reacheable value of distance, with fixed input format
	occ=(int *) calloc(n,sizeof(int));


	points=0;
	while(fread(tmps,sizeof(char),ROW_L,fp)>0){
		points++;
	}

	fseek(fp,0,SEEK_SET);
	point=(float ** ) malloc(sizeof(float *) * points);
	if(point==NULL ) {
		fprintf(stderr,"error allocating distances\n");
		exit(-1);

	}

	int i=0;
	while((read_char =fread(work_string,sizeof(char),BUFFER_SIZE,fp))>0&&  i<points){
		for(int k=0;k<=read_char-ROW_L;k+=ROW_L) {
			point[i]=(float *) malloc(sizeof(float) *3);
			memcpy(tmps,work_string+k,ROW_L);
			sscanf(tmps,"%f %f %f\n",&point[i][0],&point[i][1],&point[i][2]);
			i++;
		}


	}

#pragma omp parallel for shared (points,point) private (dist) reduction(+:occ[:n])
	for(int i=0;i<points-1;i++) {
		for(int j=i+1;j<points;j++) {

			dist=roundf(100*sqrtf( (point[i][0]-point[j][0])*(point[i][0]-point[j][0])+(point[i][1]-point[j][1])*(point[i][1]-point[j][1]) + (point[i][2]-point[j][2])*(point[i][2]-point[j][2])  ));
			//		printf("%d\n",dist);

			occ[dist]++;				


		}

	}



	for(int i=0;i<n;i++) {
		if(i<1000) {
			fprintf(stdout,"0%.2f %d\n",((float)(i))/100,occ[i]);

		}else {
			fprintf(stdout,"%.2f %d\n",((float)(i))/100,occ[i]);

		}

	}

	free(occ);
	free(tmps);
	fclose(fp);
	if(fp==NULL ) {
		fprintf(stderr,"error closing the file\n");
		exit(-1);
	}

	return 0;
}

