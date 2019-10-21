#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define CL_KERNEL_FILE "./heat_diffusion.cl"
#define INPUT_FILE "./diffusion"
#define MAX_KERNEL_SIZE 31457280 // 30 Mbytes



/*open cl version 1.2*/
#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif


#include <CL/cl.h>



int main (int argc , char ** argv ) 
{
	int required_iterations=0;
	float diffusion_const=0;
	int ret;
	int option=0;
	int h,w,x,y;
	float  ** matrix,val,avg;
	FILE *fp;
	char * opencl_program_src;
	long int size_cl_program=0;
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

	avg=0;
	fp=fopen(INPUT_FILE,"r");
	if(fp==NULL) {
		fprintf(stderr,"error openign the file \n");
		exit(-1);	
	}
	/*saving input files*/
	/*adding two additional rows and colums for avoididn to use control on the indexes*/
	fscanf(fp,"%d %d\n",&h,&w);
	h+=2;
	w+=2;
	matrix=(float **) malloc ( sizeof(float  *) *h);
	if(matrix==NULL) {
		fprintf(stderr,"error allocating matrix\n");
		exit(-1);
	}

	for(int i=0;i<h;i++) {
		matrix[i]=(float *) malloc (sizeof(float) * (w));
		if(matrix[i]==NULL)  {
			fprintf(stderr,"errorr allocating row\n");
			exit(-1);

		}
		/*obtaining a clean matrix and with boundaries at zero*/
		for(int j=0;j<w;j++) {
			matrix[i][j]=0;

		}



	}
	/*populating matrix reading from file*/
	while(fscanf(fp,"%d %d %lf\n",&x,&y,&val)!=EOF) {
		matrix[x][y]=val;

	}

	fclose(fp);
	if(fp==NULL) {
		fprintf(stderr,"error closing the file\n");
		exit(-1);
	}
	/*loading the kernel file*/
	fp=fopen(CL_KERNEL_FILE,"r");
	if(fp==NULL) {
		fprintf(stderr,"error opening .cl file\n");
		exit(-1);
	}


	opencl_program_src=(char *)calloc(MAX_KERNEL_SIZE,sizeof(char));
	if(opencl_program_src==NULL) {
		fprintf(stderr,"error allocating string for opencl program\n");
		exit(-1);
	}

	size_cl_program=fread(opencl_program_src,sizeof(char),MAX_KERNEL_SIZE,fp);	


	fclose(fp);
	if(fp==NULL) {
		fprintf(stderr,"error closing .cl file\n");
		exit(-1);	
	}
	/*******************/
	/*SET UP open cl*/
	/****************/

	/*data types of open cl declaration*/
	cl_int error;
	cl_platform_id platform_id;
	cl_uint nmb_platforms;
	cl_program program;
	cl_context context;
	cl_command_queue command_queue;
	cl_kernel kernel;
	cl_device_id device_id;
	cl_uint nmb_devices;

	/*platform*/
	if (clGetPlatformIDs(1, &platform_id, &nmb_platforms) != CL_SUCCESS) {
		printf( "cannot get platform\n" );
		return 1;
	}



	/*device */	

	if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1,
				&device_id, &nmb_devices) != CL_SUCCESS) {
		printf( "cannot get device\n" );
		return 1;
	}

	/*context*/


	cl_context_properties properties[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties) platform_id,
		0
	};

	context = clCreateContext(properties,1, &device_id, NULL, NULL, &error);
	if (error != CL_SUCCESS) {
		printf("cannot create context\n");
		return 1;
	}
	/*command queue-> for sendig comman to the device 
	 *
	 *The command queue takes memory writes and reads, and kernel execution
	 * 
	 * i.e. execution is controlled by command queue
	 * */

	command_queue = clCreateCommandQueue(context, device_id, 0, &error);
	if (error != CL_SUCCESS) {
		printf("cannot create command queue\n");
		return 1;
	}


	/*open cl program command stored in a string */
	program = clCreateProgramWithSource(context,1,
			(const char **) &opencl_program_src, NULL, &error);

	/*error checking -> build log of the kernel*/

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
		exit(-1);
	}
	kernel = clCreateKernel(program, "compute_diffusion", &error);
	if(error!=CL_SUCCESS) {
		fprintf(stderr,"cannot build kernel\n");
		exit(-1);
	}
	//TODO
	/*memory reserved as buffers*/
	  const size_t ix_m = 10e7;
	  cl_mem input_buffer_a, input_buffer_b, output_buffer_c;
	  input_buffer  = clCreateBuffer(context, CL_MEM_READ_ONLY,
	  sizeof(float) * ix_m, NULL, &error);
	  output_buffer_avg  = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
	  sizeof(float) * ix_m, NULL, &error);
	  output_buffer_matrix = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
	  sizeof(float) * ix_m, NULL, &error);


	/*three last arguments are used for synchronization ( events ) */
	/*enqueuing writing buffers*/
		clEnqueueWriteBuffer(command_queue, input_buffer_a, CL_TRUE,
		0, ix_m*sizeof(float), a, 0, NULL, NULL);
		clEnqueueWriteBuffer(command_queue, input_buffer_b, CL_TRUE,
		0, ix_m*sizeof(float), b, 0, NULL, NULL);

/*
 * ATTENTION: real index of matrix goes from 1 to n-1
 */

	
	//TODO 	CHECK
		for(size_t step=0;step<required_iterations ;step++) {
		/*enqueuing kernel */
		clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer_a);
		clSetKernelArg(kernel, 1, sizeof(cl_mem), &input_buffer_b);
		clSetKernelArg(kernel, 2, sizeof(cl_mem), &output_buffer_c);
		clEnqueueNDRangeKernel(command_queue, kernel, dimension, 
				global_offsets, global_sizes, local_sizes, 0, NULL, NULL);



		}
		
	/*print average temperature*/

	avg=avg/(h*w);
	fprintf(stdout,"Average temperature: %f\n",avg);

	/*print absolute difference between entries of matrix and average tmp*/

	for(int i=0;i<h;i++) {
		for(int j=0;j<w;j++) {
			fprintf(stdout,"%f ",abs(matrix[i][j]-avg));
		}
		fprintf(stdout,"\n");


	}
	/*
	   clReleaseMemObject(input_buffer_a);
	   clReleaseMemObject(input_buffer_b);
	   clReleaseMemObject(output_buffer_c);*/




	/*release memory*/
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	for(int i=0;i<h;i++) {
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}


