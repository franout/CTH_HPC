#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#define INPUT_FILE "./coordinates.txt"
#define ROW_L 24// length of row  + \n
#define BUFFER_SIZE 2048 
#define EPS 2
/*ordered linked list definition */
typedef struct node * node_p;

struct node {
	int occ;
	char value[5+1]; // length of string + \0
	node_p next;
};


// function for free memory 
static void free_list(node_p  element) ;

static void add_to_list(node_p  head, double value);



int main (int argc , char ** argv ) {

	int N_THREAD=0;
	int option=0;
	FILE * fp,*fp_1;
	double dist,x1,x2,y1,y2,z1,z2;
	char* work_string,* work_string_2; // +\n + \0
	node_p head=NULL;
	node_p x;
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
	work_string=(char *)calloc(ROW_L+1, sizeof(char));
	work_string_2=(char *)calloc(ROW_L+1,sizeof(char));
	if(work_string==NULL || work_string_2==NULL) {
	fprintf(stderr,"error allocating strings\n");
	exit(-1);
	}
	fp=fopen(INPUT_FILE,"r");
	fp_1=fopen(INPUT_FILE,"r");
	if(fp==NULL || fp_1==NULL) {
		fprintf(stderr,"errror opening the file\n");
		exit(-1);
	}
	int i=1;
	fseek(fp_1,i*(ROW_L),SEEK_SET);

	while ((fread( work_string,sizeof(char), ROW_L,fp ))>0) {	
		sscanf(work_string,"%lf %lf %lf\n",&x1,&y1,&z1);
				int j=1;
	while(fread(work_string_2,sizeof(char),ROW_L,fp_1)>0) {
			sscanf(work_string_2,"%lf %lf %lf\n",&x2,&y2,&z2);
			dist=sqrt(pow(x2-x1 ,2)+pow(y2-y1,2)+pow(z2-z1,2));
//			add_to_list(head,dist);
	printf("dist - %.2lf\n",dist);
				j++;
		}
		
		fseek(fp_1,-(ROW_L)*(j-2),SEEK_CUR);

		i++;
	}


	// fashion way 
	//	float a = 37.777779;
	//
	//	int b = a; // b = 37    
	//	float c = a - b; // c = 0.777779   
	//	c *= 100; // c = 77.777863   
	//	int d = c; // d = 77;    
	//	a = b + d / (float)100; // a = 37.770000;


	// printing ordered list 
	for(x=head; x!=NULL;x=x->next) {
		fprintf(stdout,"%s %d\n", x->value,x->occ);
	}




	free_list(head);
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

static void add_to_list(node_p  head, double value){
	node_p x,new_node;
	char str[6];
	
	
	
	for (x=head;x->next!=NULL;x=x->next) {
	
	
	
	}
	

	return ;
}


static void free_list(node_p element) {

	if(element==NULL) {
		return;
	}

	free_list(element->next);
	free(element);
}
