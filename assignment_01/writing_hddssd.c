#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#define PATH_SSD "/run/mount/scratch/hpcuser018/myfile.txt"
#define PATH_HDD "./myfile.txt"
#define N 1048576


int main (void) {


	FILE * fp_ssd, *fp_hdd;
	int i, dummy;
	struct timespec ts_end,ts_start;
	double time_ms;
/* using fprint and fscanf*/
	
	fp_hdd=fopen(PATH_HDD,"w");
	if ( fp_hdd== NULL) {
		fprintf(stderr,"error opening file  pointer in hdd\n");
		exit(-1);
	}

	fp_ssd=fopen(PATH_SSD,"w");
	if ( fp_ssd== NULL) {
		fprintf(stderr,"error opening file pointer in ssd\n");
		exit(-1);
	}

	
	fprintf(stdout,"fprinft and fscanf performance :\n");

	fprintf(stdout,"****************** writing to hdd ******************\n");
	time_ms=0;
	for ( i=0;i<N;i++){

	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}
	fprintf(fp_hdd,"%d",i);	
	
		if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}

	time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;

	}
		fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);

	fprintf(stdout, "****************** writing to ssd ******************\n");
	time_ms=0;
	for ( i=0;i<N;i++){

	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}
	fprintf(fp_ssd,"%d",i);
	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}
	time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


	}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);


	fclose(fp_ssd);
	fclose(fp_hdd);
	if ( fp_ssd== NULL || fp_hdd == NULL) {
	fprintf(stderr,"error closing the files \n ");
	exit(-1);
	}


	fp_hdd=fopen(PATH_HDD,"w");
	if ( fp_hdd== NULL) {
		fprintf(stderr,"error opening file in hdd\n");
		exit(-1);
	}

	fp_ssd=fopen(PATH_SSD,"w");
	if ( fp_ssd== NULL) {
		fprintf(stderr,"error opening file in ssd\n");
		exit(-1);
	}


	fprintf(stdout,"****************** reading from hdd ******************\n");
	time_ms=0;
	for ( i=0;i<N;i++){
		if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}

	fscanf(fp_hdd,"%d",&dummy);
	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}
		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;



	}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);




	fprintf(stdout,"****************** reading from ssd******************\n");
	time_ms=0;
	for ( i=0;i<N;i++){
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}


	fscanf(fp_ssd,"%d",&dummy);	
	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}
		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;

	}


	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);



	fclose(fp_ssd);
	if(fp_ssd== NULL) {
		fprintf(stderr,"error closing the file \n");
	exit(-1);
	}

	fclose(fp_hdd);
	if(fp_hdd==NULL){
	fprintf(stderr,"error closing the file \n");
	exit(-1);
	}
/* performance usign fwrite and fread ( bufferizing the input of fucntion ) */

	fp_hdd=fopen(PATH_HDD,"w");
	if ( fp_hdd== NULL) {
		fprintf(stderr,"error opening file  pointer in hdd\n");
		exit(-1);
	}

	fp_ssd=fopen(PATH_SSD,"w");
	if ( fp_ssd== NULL) {
		fprintf(stderr,"error opening file pointer in ssd\n");
		exit(-1);
	}

	
	fprintf(stdout,"fwrite and fread performance :\n");

	fprintf(stdout,"****************** writing to hdd ******************\n");
	time_ms=0;

	for ( i=0;i<N;i++){
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}
	
		fwrite(&i, sizeof(i)  ,1 , fp_hdd );
	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}
	
	
		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


	}

	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);



	fprintf(stdout, "****************** writing to ssd ******************\n");

	for ( i=0;i<N;i++){
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}
	fwrite(&i,sizeof(i), 1, fp_ssd);

	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}
	
		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


		}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);

	fclose(fp_ssd);
	fclose(fp_hdd);
	if ( fp_ssd== NULL || fp_hdd == NULL) {
	fprintf(stderr,"error closing the files \n ");
	exit(-1);
	}


	fp_hdd=fopen(PATH_HDD,"w");
	if ( fp_hdd== NULL) {
		fprintf(stderr,"error opening file in hdd\n");
		exit(-1);
	}

	fp_ssd=fopen(PATH_SSD,"w");
	if ( fp_ssd== NULL) {
		fprintf(stderr,"error opening file in ssd\n");
		exit(-1);
	}


	fprintf(stdout,"****************** reading from hdd ******************\n");
	time_ms=0;
	for ( i=0;i<N;i++){
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}
	
	fread(&dummy,sizeof(dummy) , 1,fp_hdd);
	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}

		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


		}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);


	fprintf(stdout,"****************** reading from ssd******************\n");
	time_ms=0;
	for ( i=0;i<N;i++){
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}
	fread(&dummy , sizeof(dummy),1,fp_ssd);
	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}

		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


		}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);




	fclose(fp_ssd);
	if(fp_ssd== NULL) {
		fprintf(stderr,"error closing the file \n");
	exit(-1);
	}

	fclose(fp_hdd);
	if(fp_hdd==NULL){
	fprintf(stderr,"error closing the file \n");
	exit(-1);
	}


/*  performances uisng write and read */

	int fd_ssd, fd_hdd;
		
	fprintf(stdout,"write and read performance :\n");

	
	if ( (	fd_hdd= open (PATH_HDD,O_WRONLY | O_CREAT | O_TRUNC)	) == -1 ) {
	fprintf(stderr,"error opening the file in hdd \n");
	exit ( -1);
	}
	if ((fd_ssd = open (PATH_SSD,O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
	fprintf(stderr,"error opening the file in ssd \n");
	exit(-1);
	}
	fprintf(stdout,"****************** writing to hdd ******************\n");

	time_ms=0;
	for ( i=0;i<N;i++){
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}

		if ( write(fd_hdd,&i ,sizeof(i) ) == -1)	{
		fprintf(stderr,"error writing the file \n");
		exit(-1);
		}

	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}

		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


		}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);



	fprintf(stdout,"****************** writing to ssd ******************\n");
	time_ms=0;
	for ( i=0;i<N;i++){

 

	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}      
		if ( write(fd_ssd,&i,sizeof(i)) == -1 ) {
	 	fprintf(stderr,"error wrinting to the file in the ssd \n");
		exit(-1);
	 }
		if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}
	
		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


		}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);


	if (close(fd_ssd)== -1) {
	fprintf(stderr,"error closing file on ssd \n");
	exit(-1);
	}
	if ( close(fd_hdd) == -1) {
	fprintf(stderr,"error closing file on hdd \n");
	exit(-1);
	}


	if ( (	fd_hdd= open (PATH_HDD,O_RDONLY)) == -1 ) {
	fprintf(stderr,"error opening the file in hdd \n");
	exit ( -1);
	}
	if ((fd_ssd = open (PATH_SSD,O_RDONLY)) == -1) {
	fprintf(stderr,"error opening the file in ssd \n");
	exit(-1);
	}

	fprintf(stdout,"****************** reading from hdd ******************\n");
	time_ms=0;

	for ( i=0;i<N;i++){
		
	
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}	
		if( read(fd_hdd, &dummy, sizeof(i)) == -1 ) {
		fprintf(stderr,"erorr reading the file in the hdd\n");
		exit(-1);		
		}
	
	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}
	
		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


		}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);



	fprintf(stdout,"****************** reading from ssd ******************\n");

	time_ms=0;
	for ( i=0;i<N;i++){
	if(!timespec_get(&ts_start,TIME_UTC)){
	fprintf(stderr,"errro during the acquisition of start time \n");
	exit(-1);
	}
	
		if( read(fd_ssd,&dummy,sizeof(i)) == -1) {
		fprintf(stderr,"erorr reading the file in the ssd\n");
		exit(-1);
		}

	if(!timespec_get(&ts_end,TIME_UTC)){
	fprintf(stderr,"error during the acquisition of end time \n");
	exit(-1);
	}

		time_ms+=ts_end.tv_sec*1000 + ((double)ts_end.tv_nsec)/1000000 - ts_start.tv_sec*1000 - ((double)ts_start.tv_nsec)/1000000;


		}
	
	fprintf(stdout,"Execution time: %f [ms]\n\n\n", ((double)time_ms)/N);



	if (close(fd_ssd)== -1) {
	fprintf(stderr,"error closing file on ssd \n");
	exit(-1);
	}
	if ( close(fd_hdd) == -1) {
	fprintf(stderr,"error closing file on hdd \n");
	exit(-1);
	}

	//TODO : find a way to use the dma could be an improvement in a multithread enviroment?

/*
 *https://stackoverflow.com/questions/15079049/write-vs-fprintf-why-different-and-which-is-better

 for reason about the different performance
 *
 * */

return 0;
	}


