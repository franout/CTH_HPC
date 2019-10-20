#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#define INPUT_FILE "./cells"
#define ROW_L 24// length of row  + \n
#define BUFFER_SIZE 262143984
typedef struct {
	int * occ;

	int n;

}table_t;

static table_t map;
static float ** distances;


int main (int argc , char ** argv ) {

	int N_THREAD=0;
	int option=0;
	float x1,x2,y1,y2,z1,z2;
	int	dist,points;
	char* work_string,* work_string_2,*tmps; // +\n + \0
	long int read_char,read_char_2;
	FILE * fp,*fp_1;

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
	work_string_2=(char *)calloc(BUFFER_SIZE+1,sizeof(char));

	tmps=(char *) calloc(ROW_L+1,sizeof(char));
	if(work_string==NULL || work_string_2==NULL || tmps==NULL) {
		fprintf(stderr,"error allocating strings\n");
		exit(-1);
	}
	fp=fopen(INPUT_FILE,"r");
	fp_1=fopen(INPUT_FILE,"r");
	if(fp==NULL || fp_1==NULL) {
		fprintf(stderr,"errror opening the file\n");
		exit(-1);
	}
	map.n=3465+1; // fp *100 -> max reacheable value of distance, with fixed input format
	map.occ=(int *) malloc(sizeof(int)*map.n);
	for(int i=0;i<map.n;i++){
		map.occ[i]=0;
	}



points=1;
while(fread(tmps,sizeof(char),ROW_L,fp)>0){
	points++;
}

fseek(fp,0,SEEK_SET);
distances=(float ** ) malloc(sizeof(float *) * points);
if(distances==NULL ) {
	fprintf(stderr,"error allocating distances\n");
	exit(-1);

}

for(int i=0;i<points;i++) {
	distances[i]=(float*) malloc (sizeof(float) *3) ;
	if(distances[i]==NULL) 
	{
		fprintf(stderr,"error allocating row\n");
		exit(-1);
	}
	fread(tmps,sizeof(char),ROW_L,fp);
	sscanf(tmps,"%f %f %f\n",&distances[i][0],&distances[i][1],&distances[i][2]);

}

#pragma omp parallel for shared ( map,points) private (x2,y2,z2,x1,y1,z1,dist) schedule(static, 50)
for(int i=0;i<points-1;i++) {
	x2=distances[i][0];
	y2=distances[i][1];
	z2=distances[i][2];
	for(int j=i+1;j<points;j++) {
		x1=distances[j][0];
		y1=distances[j][1];
		z1=distances[j][2];
		dist=roundf(100*sqrtf((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1)) );
//		printf("%d\n",dist);
#pragma omp atomic
		map.occ[dist]++;				


	}

}



for(int i=0;i<map.n;i++) {
	int occ=map.occ[i];
	if(occ>0){
		if(i<1000) {
			fprintf(stdout,"0%.2f %d\n",((float)(i))/100,occ);

		}else {
			fprintf(stdout,"%.2f %d\n",((float)(i))/100,occ);

		}
	}
}

free(map.occ);
free(tmps);
free(work_string);
free(work_string_2);
fclose(fp);
fclose(fp_1);
if(fp==NULL || fp_1 ==NULL) {
	fprintf(stderr,"error closing the file\n");
	exit(-1);
}

return 0;
}

