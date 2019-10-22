#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define CL_KERNEL_FILE "./heat_diffusion.cl"
#define INPUT_FILE "./try.txt"
#define MAX_KERNEL_SIZE 31457280 // 30 Mbytes
#define DEBUG 1


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
	while(fscanf(fp,"%d %d %f\n",&x,&y,&val)!=EOF) {
		matrix[x+1][y+1]=val;

	}

	fclose(fp);
	if(fp==NULL) {
		fprintf(stderr,"error closing the file\n");
		exit(-1);
	}
#if DEBUG
	for(int i=0;i<h;i++) {
		for(int j=0;j<w;j++) {
			fprintf(stdout,"%.3f ",matrix[i][j]);
		}
		fprintf(stdout,"\n");
	}
#endif


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
	cl_kernel kernel_diffusion,kernel_avg,kernel_matrix_abs_val;
	cl_device_id device_id;
	cl_uint nmb_devices;
	cl_mem buffer;

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
	kernel_diffusion = clCreateKernel(program, "compute_diffusion", &error);
	if(error!=CL_SUCCESS) {
		fprintf(stderr,"cannot build kernel for diffusion calculation\n");
		exit(-1);
	}
	/*memory reserved as buffers*/
	const size_t MATRIX_SIZE = w*h; // plus the two additional row and column

	buffer  = clCreateBuffer(context, CL_MEM_READ_WRITE,
			sizeof(float) * MATRIX_SIZE, NULL, &error);
	if(error!=CL_SUCCESS) {
		fprintf(stderr,"error creating buffer\n");
		exit(-1);
	}

	/*three last arguments are used for synchronization ( events ) */
	/*enqueuing writing buffers*/
	if(clEnqueueWriteBuffer(command_queue, buffer, CL_TRUE,
				0, MATRIX_SIZE*sizeof(float) , matrix, 0, NULL, NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enqueuing the write buffer\n");
		exit(-1);

	}

	/*
	 * ATTENTION: real index of matrix goes from 1 to n-1
	 */

	error=clSetKernelArg(kernel_diffusion, 0, sizeof(cl_mem), &buffer);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting first argument of diffusion kernel\n");
		exit(-1);
	}
	error=clSetKernelArg(kernel_diffusion,1,sizeof(float),&diffusion_const);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting second argument of diffusion kernel\n");
		exit(-1);
	}
	error=clSetKernelArg(kernel_diffusion,2,sizeof(int),&h);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting third argument of diffusion kernel\n");
		exit(-1);
	}
	error=clSetKernelArg(kernel_diffusion,3,sizeof(int),&w);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting fourth argument of diffusion kernel\n");
		exit(-1);
	}
	const size_t global[] = {h,w};
	/*executing the required iterations*/
	for(size_t step=0;step<required_iterations ;step++) {

		error=clSetKernelArg(kernel_diffusion,4,sizeof(int), &step);
		if(error!=CL_SUCCESS) {
			fprintf(stderr,"error setting fiveth argument of diffusion kernel\n");
			exit(-1);
		}
		/*enqueuing kernel */
		clEnqueueNDRangeKernel(command_queue, kernel_diffusion, 2, 
				NULL, (const size_t *)&global, NULL, 0, NULL, NULL);
		// null after matrix size will allow to choose automatically the size


	}

	/*SYNCHRONIZATION POINT*/
	/*it will wait until there are still some commands in the queue*/
	if(clFinish(command_queue)!=CL_SUCCESS) {
		fprintf(stderr,"error in finishing the command queue\n");
		exit(-1);	
	}
	/*reading the output diffusion matrix*/
	/*enqueuing reading buffer, implicit waiting thanks to CL_TRUE*/	
	if(clEnqueueReadBuffer(command_queue,buffer,CL_TRUE,0,MATRIX_SIZE*sizeof(float),matrix,0,NULL,NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enqueuing the read buffer\n");
		exit(-1);
	}



#if DEBUG
	printf("%d %d size\n",h,w);	
	for(int i=0;i<h;i++) {
		for(int j=0;i<w;j++) {
			fprintf(stdout,"%.2f ",matrix[i][j]);

		}
		fprintf(stdout,"\n");

	}

#endif
	avg=0;
	/*instantiate kernel for computing the average ( also arguments ) */
	kernel_avg=clCreateKernel(program,"compute_average",&error);
	if(error!=CL_SUCCESS){
		fprintf(stderr,"cannot build kernel for average calculation\n");
		exit(-1);
	}
	buffer_out  = clCreateBuffer(context, CL_MEM_READ_WRITE,
			sizeof(float) , NULL, &error);
	if(error!=CL_SUCCESS) {
		fprintf(stderr,"error creating buffer\n");
		exit(-1);
	}


	/*enqueuing writing buffers*/
	if(clEnqueueWriteBuffer(command_queue, buffer, CL_TRUE,
				0, MATRIX_SIZE*sizeof(float) , matrix, 0, NULL, NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enqueuing the write buffer\n");
		exit(-1);

	}
	if(clEnqueueWriteBuffer(command_queue,buffer_out,CL_TRUE,0,sizeof(float),&avg,0,NULL,NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enquieung write buffer for avg\n");
		exit(-1);
	}
	/*setting arguments*/

	error=clSetKernelArg(kernel_avg, 0, sizeof(cl_mem), &buffer);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting first argument of avg kernel\n");
		exit(-1);
	}

	error=clSetKernelArg(kernel_avg,1,sizeof(int),&h);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting second argument of avg kernel\n");
		exit(-1);
	}
	error=clSetKernelArg(kernel_avg,2,sizeof(int),&w);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting third argument of avg kernel\n");
		exit(-1);
	}
	error=clSetKernelArg(kernel_avg,3,sizeof(cl_mem),&buffer_out);
	if(error!=CL_SUCCESS){
		fprintf(stderr,"error setting the fourth argument of avg kernel\n");
		exit(-1);
	}
	/*executing kernel*/
	clEnqueueNDRangeKernel(command_queue, kernel_avg, 2, 
			NULL, (const size_t *)&global, NULL, 0, NULL, NULL);

	/*it will wait until there are still some commands in the queue*/
	if(clFinish(command_queue)!=CL_SUCCESS) {
		fprintf(stderr,"error in finishing the command queue\n");
		exit(-1);	
	}

	/*print average temperature*/
	if(clEnqueueReadBuffer(command_queu,buffer_out,CL_TRUE,0,sizeof(float),&avg,0,NULL,NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enqueuing the read buffer for avg\n");
		exit(-1);
	}

	fprintf(stdout,"Average temperature: %.2f\n",avg);


#if DEBUG
	avg=0;
	for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
			avg+=matrix[i][j];	
		}

	}
	avg/=(h*w);
	fprintf(stdout,"Average temperature: %.2f\n",avg);

#endif


	/* instantiate kernel for computing the matrix absolute difference of each temperature with the average ( also args ) */
	kernel_matrix_abs_val=clCreateKernel(program,"compute_matrix_abs_val",&error);
	if(error!=CL_SUCCESS) {
		fprintf(stderr,"cannot build kernle for computing absolute values\n");
		exit(-1);
	}
	/*copying in the device memory*/
	/*enqueuing writing buffers*/
	if(clEnqueueWriteBuffer(command_queue, buffer, CL_TRUE,
				0, MATRIX_SIZE*sizeof(float) , matrix, 0, NULL, NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enqueuing the write buffer foor abs matrix\n");
		exit(-1);

	}
	if(clEnqueueWriteBuffer(command_queue,buffer_out,CL_TRUE,0,sizeof(float),&avg,0,NULL,NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enquieung write buffer for abs matrix\n");
		exit(-1);
	}
	/*setting arguments*/

	error=clSetKernelArg(kernel_matrix_abs_val, 0, sizeof(cl_mem), &buffer);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting first argument of abs matrix kernel\n");
		exit(-1);
	}

	error=clSetKernelArg(kernel_matrix_abs_val,1,sizeof(int),&h);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting second argument of abs matrix kernel\n");
		exit(-1);
	}
	error=clSetKernelArg(kernel_matrix_abs_val,2,sizeof(int),&w);
	if(error!=CL_SUCCESS)	{
		fprintf(stderr,"error setting third argument of abs matrix kernel\n");
		exit(-1);
	}
	error=clSetKernelArg(kernel_matrix_abs_val,3,sizeof(cl_mem),&buffer_out);
	if(error!=CL_SUCCESS){
		fprintf(stderr,"error setting the fourth argument ofabs matrix kernel\n");
		exit(-1);
	}
	/*executing kernel*/
	clEnqueueNDRangeKernel(command_queue, kernel_matrix_abs_val, 2, 
			NULL, (const size_t *)&global, NULL, 0, NULL, NULL);

	/*it will wait until there are still some commands in the queue*/
	if(clFinish(command_queue)!=CL_SUCCESS) {
		fprintf(stderr,"error in finishing the command queue\n");
		exit(-1);	
	}

	if(clEnqueueReadBuffer(command_queu,buffer,CL_TRUE,0,sizeof(float)*MATRIX_SIZE,matrix,0,NULL,NULL)!=CL_SUCCESS) {
		fprintf(stderr,"error enqueuing the read buffer for abs matrix\n");
		exit(-1);
	}

	/*print absolute difference between entries of matrix and average tmp*/

	for(int i=1;i<h-1;i++) {
		for(int j=1;j<w-1;j++) {
#if DEBUG
			fprintf(stdout,"%f ",abs(matrix[i][j]-avg));
#endif
		}
		fprintf(stdout,"\n");


	}





	/*release memory*/


	clReleaseMemObject(buffer);
	clReleaseMemObject(buffer_out);
	clReleaseProgram(program);
	clReleaseKernel(kernel_diffusion);
	clReleaseKernel(kernel_avg);
	clReleaseKernel(kernel_matrix_abs_val);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	for(int i=0;i<h;i++) {
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}


