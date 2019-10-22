#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ( int argc, char ** argv ) {

int final_a,final_b=0;
int option=0;
/*variables from  unistd*/	
// int opterr, optopt,optind;
//char * optarg;





/*it returns the parsed char and as third arg it requires the separation char for arguments*/
while ((option = getopt(argc,argv,"a:b:"))!=-1) {

	switch(option){
	
		case 'a':
		sscanf(optarg,"%d",&final_a);		
			break;

		case 'b':
			sscanf(optarg,"%d",&final_b);
			break;
		default:
			exit(-1);
	
	}




}
fprintf(stdout,"A is %d and B is %d\n", final_a,final_b);
return 0 ;
}
