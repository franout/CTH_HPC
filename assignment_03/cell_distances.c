#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#define INPUT_FILE "./cells"
#define ROW_L 24// length of row  + \n
#define BUFFER_SIZE 2400

typedef struct {
	int * occ;

	int n;

}table_t;

static table_t map;



int main (int argc , char ** argv ) {

	int N_THREAD=0;
	int option=0;
	float x1,x2,y1,y2,z1,z2;
	 int	dist;
	char* work_string,* work_string_2,*tmps,*tmps2; // +\n + \0
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
	tmps=(char *)calloc(ROW_L+1, sizeof(char));
	tmps2=(char *) calloc(ROW_L+1,sizeof(char));
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

	x1=x2=y2=y1=z1=z2=read_char=read_char_2=0;
	fseek(fp_1,(ROW_L),SEEK_SET);

	while ((read_char=fread( work_string,sizeof(char), BUFFER_SIZE,fp ))>0) {
		int j=1;
		while((read_char_2=fread(work_string_2,sizeof(char),BUFFER_SIZE,fp_1))>0) {
			// parallel part
#pragma omp single

			// if too slow switch second reading loop with the parsing buffer loop
			for(int kb=0;kb<read_char-ROW_L;kb+=ROW_L) {
				memcpy(tmps,work_string+kb,ROW_L);
				sscanf(tmps,"%f %f %f\n",&x2,&y2,&z2);
#pragma omp parallel for shared(kb,x2,y2,z2,read_char_2,map,work_string_2) private(x1,y1,z1,dist)

				for(int kb_i=kb;kb_i<read_char_2;kb_i+=ROW_L) {
					memcpy(tmps2,work_string_2+kb_i,ROW_L);
					sscanf(tmps2,"%f %f %f\n",&x1,&y1,&z1);
					//TODO error in approximation of numbers
					dist=round(sqrt(10* ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1)) ));
/*					fprintf(stdout,"%lf %f  %f\n",sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1) ),sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1) ),sqrtf( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1) ));*/
#pragma omp atomic
					map.occ[dist]++;				
				}
			}
			j++;

		}
		fseek(fp_1,-(ROW_L)*(j-2),SEEK_CUR);
	}

	for(int i=0;i<map.n;i++) {
		int occ=map.occ[i];
		if(occ>0){
			if(i<1000) {
				fprintf(stdout,"0%.2f %d\n",((float)(i))/100,occ);

			}else {
				fprintf(stdout,"%.2f %d\n",((float)(i))/100,occ);

			}}
	}
	free(map.occ);
	free(tmps);
	free(tmps2);
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

