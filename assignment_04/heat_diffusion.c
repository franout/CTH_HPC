#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#define INPUT_FILE "./diffusion_100_100"



int main (int argc , char ** argv ) 
{
	int required_iterations=0;
	double diffusion_const=0;
	int ret;
	int option=0;
	int h,w,x,y;
	double ** matrix,val;
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

	fp=fopen(INPUT_FILE,'r');
	if(fp==NULL) {
		fprintf(stderr,"error openign the file \n");
		exit(-1);	
	}
	/*saving input files*/
	fscanf(fp,"%d %d\n",&h,&w);
	matrix=(double **) malloc ( sizeof(double *) *h);
	if(matrix==NULL) {
		fprintf(stderr,"error allocating matrix\n");
		exit(-1);
	}
	for(int i=0;i<h;i++) {
		matrix[i]=(double *) malloc (sizeof(double) * w);
		if(matrix[i]==NULL)  {
			fprintf(stderr,"errorr allocating row\n");
			exit(-1);

		}
		fscanf(fp,"%d %d %lf\n",&x,&y,&val);
		matrix[x][y]=val;
	}	
	fclose(fp);
	if(fp==NULL) {
	fprintf(stderr,"error closing the file\n");
	exit(-1);
	}


	/*device */	
	cl_int error;

	cl_platform_id platform_id;
	cl_uint nmb_platforms;
	if (clGetPlatformIDs(1, &platform_id, &nmb_platforms) != CL_SUCCESS) {
		printf( "cannot get platform\n" );
		return 1;
	}

	/*platform*/
	cl_device_id device_id;
	cl_uint nmb_devices;
	if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1,
				&device_id, &nmb_devices) != CL_SUCCESS) {
		printf( "cannot get device\n" );
		return 1;
	}
	/*context*/

	cl_context context;
	cl_context_properties properties[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties) platform_id,
		0
	};
	context = clCreateContext(properties, 1, &device_id, NULL, NULL, &error);


	clReleaseContext(context);


	/*command queue*/

	cl_command_queue command_queue;
	command_queue = clCreateCommandQueue(context, device_id, 0, &error);
	if (error != CL_SUCCESS) {
		printf("cannot create context\n");
		return 1;
	}

	clReleaseCommandQueue(command_queue);

	/*open cl program command stored in a string */

	cl_program program;
	program = clCreateProgramWithSource(context,1,
			(const char **) &opencl_program_src, NULL, &error);

	clBuildProgram(program, 0, NULL, NULL, NULL, NULL)

		cl_kernel kernel;
	kernel = clCreateKernel(program, "dot_prod_mul", &error);

	clReleaseProgram(program);
	clReleaseKernel(kernel);


	/*error checjign*/

	error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (error != CL_SUCCESS) {
		printf("cannot build program. log:\n");

		size_t log_size = 0;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
				0, NULL, &log_size);

		char * log = calloc(log_size, sizeof(char));
		if (log == NULL) {
			printf("could not allocate memory\n");
			return 1;
		}

		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
				log_size, log, NULL);
		printf( "%s\n", log );

		free(log);

		return 1;


		/*memory reserved as buffers*/

		//TODO use opencl data types
		




	for(int i=0;i<h;i++) {
	free(matrix[i]);
	}
	free(matrix);

		return 0;
	}



	/*kernel is afunction or a task to do Saved as a separate file (typically ending in .cl) and loaded on runtime */

	__kernel void
		dot_prod_mul(
				__global const float * a,
				__global const float * b,
				__global float * c
			    )
		{
			int ix = get_global_id(0);
			c[ix] = a[ix] * b[ix];
		}
