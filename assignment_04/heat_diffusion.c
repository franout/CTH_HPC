#include <stdio.h>
#include <stdlib.h>
#include ""


int main (int argc , char ** argv ) 
{

	/*

	   A simple model for heat diffiusion in 2-dimensional space, splits it up into n_x times n_x small boxes. In each time step, the temperature h(i,j) of a box with coodinates i and j is updated as

	   h(i,j) + c * ( (h(i-1,j) + h(i+1,j) + h(i,j-1) + h(i,j+1))/4 - h(i,j) )

	   where c is a diffision constant. We consider the case of bounded region of box shape whose boundary has constant temperature 0.
	   Implement in C using the CPU via OpenCL a program heat_diffusion that

	   Reads array size and initial values from the file diffusion.
	   Executes a given number of steps of heat diffusion with given diffusion constant, and
	   Outputs the average a of temperatures.
	   Outputs the average absolute difference of each temperature to the average of all temperatures.

*/

	int required_iterations=0;
	double diffusion_const=0;
	int ret;
	int option=0;

	/*it returns the parsed char and as third arg it requires the separation char for arguments*/
	while ((option = getopt(argc,argv,"n:d:"))!=-1) {

		switch(option){

			case 'n':
				sscanf(optarg,"%d",&required_iterations);		
				break;

			case 'd':
				sscanf(optarg,"%f",&diffusion_const);
				break;

			default:
				fprintf(stdout,"no valid arguments\n");
				exit(-1);

		}
	}






	return 0;
}
