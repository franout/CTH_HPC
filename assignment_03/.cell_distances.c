#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#define INPUT_FILE "./coordinates.txt" 
#define ROW_L 24// length of row  + \n
#define BUFFER_SIZE 4096
/*ordered linked list definition */
typedef struct node * node_p;

struct node {
	int occ;
	char value[5+1]; // length of string + \0
	node_p next;
}node;


// function for free memory
static void free_list(node_p  element) ;

static void add_to_list(node_p *  head, float  value);

static int mystrcmp ( const char * str1, const char * str2);



int main (int argc , char ** argv ) {

	int N_THREAD=0;
	int option=0;
	float x1,x2,y1,y2,z1,z2, tmp1,tmp2,tmp3;
	float dist;
	char* work_string,* work_string_2,*tmps,*tmps2; // +\n + \0
	long int read_char,read_char_2;
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
	work_string=(char *)calloc(BUFFER_SIZE, sizeof(char));
	work_string_2=(char *)calloc(BUFFER_SIZE,sizeof(char));
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
	int i=1;
	x1=y1=z1=x2=y2=z2=tmp1=tmp2=tmp3=read_char=read_char_2=0;
	fseek(fp_1,i*(ROW_L),SEEK_SET);

	while ((read_char=fread( work_string,sizeof(char), BUFFER_SIZE,fp ))>0) {
		int j=1;
		while((read_char_2=fread(work_string_2,sizeof(char),BUFFER_SIZE,fp_1))>0) {
			// parallel part
#pragma omp single 		
			// if too slow switch second reading loop with the parsing buffer loop
			for(int kb=0;kb<read_char-ROW_L;kb+=ROW_L) {
				memcpy(tmps,work_string+kb,ROW_L);

				/*sscanf(tmp,"%d.%d %d.%d %d.%d\n",&tmp1,&x2,&tmp2,&y2,&tmp3,&z2);
				  x2+=tmp1*1000;
				  y2+=tmp2*1000;
				  z2+=tmp3*1000;
				  */

				sscanf(tmps,"%f %f %f\n",&x2,&y2,&z2);

#pragma omp parallel for shared(x2,y2,z2,work_string_2,kb)
				for(int kb_i=kb;kb_i<read_char_2;kb_i+=ROW_L) {

					memcpy(tmps2,work_string_2+kb_i,ROW_L);;
					/*sscanf(tmp,"%d.%d %d.%d %d.%d\n",&tmp1,&x1,&tmp2,&y1,&tmp3,&z1);
					  x1+=tmp1*1000;
					  y1+=tmp2*1000;
					  z1+=tmp3*1000;
					// compute distance
					dist=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1)); */
					sscanf(tmps2,"%f %f %f\n",&x1,&y1,&z1);
					dist=sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(z2-z1,2));
					// add to a list CRITICAL section
					printf("%.3f\n",dist);
#pragma omp critical (list_insertion)
					// try with reductio
					// // try with reductionn
					add_to_list ( &head, dist);
				}
			}
			j++;

		}
		fseek(fp_1,-(ROW_L)*(j-2),SEEK_CUR);
		i++;
	}

	// printing ordered list
	for(x=head; x!=NULL;x=x->next) {
		fprintf(stdout,"%s %d\n", x->value,x->occ);
	}




	free_list(head);

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

static void add_to_list(node_p *head, float value){
	node_p x,y,new_node,z;
	char str[6];
	if(value<9.99) {
		sprintf(str,"0%.2f",value);
	}else {
		sprintf(str,"%.2f",value);
	}
	new_node=(node_p ) malloc(sizeof(struct node));
	new_node-> occ=1;
	strcpy(new_node->value,str);
	new_node->next=NULL;

	if(*head==NULL) {
		*head=new_node;
	}

	else {

		for(x=(*head)->next, y=*head,z=NULL;x!=NULL &&  strcmp(str,x->value)>=0 ;z =y ,y=x, x=x->next) {
			if ( strcmp(str,y->value)==0) {
				y->occ++;
				free(new_node);
				new_node=NULL;
				break;
			}

		}
		if(new_node!=NULL) {
			if(strcmp(str,y->value)>0) {
				new_node->next=x;
				y->next=new_node;


			}else if ( strcmp(str,y->value)<0) {

				new_node->next=y;
				*head=new_node;		

			}
			/*			new_node->next=x;
						y->next=new_node;
						*/
		}
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

