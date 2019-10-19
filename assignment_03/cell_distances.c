#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#define INPUT_FILE "./cell_e4"
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
	float dist;
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
	if(work_string==NULL || work_string_2==NULL || tmps==NULL|| tmps2==NULL) {
		fprintf(stderr,"error allocating strings\n");
		exit(-1);
	}
	fp=fopen(INPUT_FILE,"r");
	fp_1=fopen(INPUT_FILE,"r");
	if(fp==NULL || fp_1==NULL) {
		fprintf(stderr,"errror opening the file\n");
		exit(-1);
	}
	map.n=34640; // fp *100 -> max reacheable value of distance, with fixed input format
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


			// if too slow switch second reading loop with the parsing buffer loop
			for(int kb=0;kb<read_char-ROW_L;kb+=ROW_L) {
				memcpy(tmps,work_string+kb,ROW_L);
				sscanf(tmps,"%f %f %f\n",&x2,&y2,&z2);
				for(int kb_i=kb;kb_i<read_char_2;kb_i+=ROW_L) {

					memcpy(tmps2,work_string_2+kb_i,ROW_L);;

					sscanf(tmps2,"%f %f %f\n",&x1,&y1,&z1);

					dist=sqrtf((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
					// add to a list CRITICAL section
				//	printf("%.3f\n",dist);
					map.occ[(int)(dist*100)]++;
				}
			}
			j++;

		}
		fseek(fp_1,-(ROW_L)*(j-2),SEEK_CUR);
			}

	for(int i=0;i<map.n;i++) {
		int occ=map.occ[i];
	if(occ>=1) {
	fprintf(stdout,"%.2f %d\n",i/100.00,occ);
	
	}
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

