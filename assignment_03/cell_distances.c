#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#define INPUT_FILE "./coordinates.txt"
#define ROW_L 24// length of row  + \n
#define BUFFER_SIZE 100 
#define EPS 2
/*ordered linked list definition */
typedef struct node * node_p;

struct node {
	int occ;
	char value[5+1]; // length of string + \0
	node_p next;
}node;


// function for free memory 
static void free_list(node_p  element) ;

static void add_to_list(node_p  head, double value);



int main (int argc , char ** argv ) {

	int N_THREAD=0;
	int option=0;
	int dist,x1,x2,y1,y2,z1,z2, tmp1,tmp2,tmp3;
	char* work_string,* work_string_2,*tmp; // +\n + \0

	FILE * fp,*fp_1;
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
	work_string=(char *)calloc((ROW_L+1)*BUFFER_SIZE, sizeof(char));
	work_string_2=(char *)calloc((ROW_L+1)*BUFFER_SIZE,sizeof(char));
	tmp=(char *)calloc(ROW_L+1, sizeof(char));
	if(work_string==NULL || work_string_2==NULL || tmp==NULL) {
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
	x1=y1=z1=x2=y2=z2=tmp1=tmp2=tmp3=0;
	fseek(fp_1,i*(ROW_L),SEEK_SET);

	while ((fread( work_string,sizeof(char), ROW_L*BUFFER_SIZE,fp ))>0) {	
		int j=1;
		while(fread(work_string_2,sizeof(char),ROW_L,fp_1)>0) {
			// parallel part 
			//
			// if too slow switch second reading loop with the parsing buffer loop
			for(int kb=0;kb<BUFFER_SIZE;kb++) {
				strncpy(tmp,work_string+kb*ROW_L,ROW_L);

				sscanf(tmp,"%d.%d %d.%d %d.%d\n",&x1,&x2,&y1,&y2,&z1,&z2);		
				x2+=x1*1000;
				y2+=y1*1000;
				z2+=z1*1000;

				for(int kb_i=0;kb_i<BUFFER_SIZE;jb_i++) {
					strncpy(tmp,work_string_2+kb_i*ROW_L,ROW_L);
					sscanf(tmp,"%d.%d %d.%d %d.%d\n",&tmp1,&x1,&tmp2,&y1,&tmp3,&z1);
					x1+=tmp1*1000;
					y1+=tmp2*1000;
					z1+=tmp3*1000;
					// compute distance
					

					// add to a list CRITICAL section 	

				}


				j++;	
			}



		}
		fseek(fp_1,-(ROW_L)*(j-2),SEEK_CUR);

		i++;
	}


	// printing ordered list 
	for(x=head; x!=NULL;x=x->next) {
		fprintf(stdout,"%s %d\n", x->value,x->occ);
	}




	free_list(head);
	free(tmp);
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
	sprintf(str,"%.2f",value);

	new_node=(node_p ) malloc(sizeof( node));
	new_node-> occ=0;
	strcpy(new_node->value,str);
	new_node->next=NULL;

	if(head==NULL) 
	{
		head=new_node;

	}
	else {
		for (x=head;x->next!=NULL;x=x->next) ;
		x->next=new_node;


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
