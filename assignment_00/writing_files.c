#include <stdio.h>
#include <stdlib.h>
#define N 3

int main (void ) {

int matrix [N][N]= {{1,2,3},{4,5,6},{7,8,9}};
int r_matrix [N][N], i , j;

/*open file*/
FILE * fp = fopen("./file.txt" , "w");
if ( fp == NULL ) {
fprintf(stderr,"erorr opening the file");
return -1;
}
/*writing to file*/
for ( i = 0 ; i < N ; i ++) {
	for ( j=0; j<N ; j++ ) {
	fprintf(fp,"%d ", matrix[i][j]);
		}
	fprintf(fp,"\n");

}

/*close file*/
fclose(fp);
if ( fp== NULL) {
fprintf(stderr,"error closing the file");
return -1;
}


fp= fopen("./file.txt","r");
if ( fp== NULL) {
fprintf(stderr,"error opening the file\n");
return -1;
}

/*read the matrix */
for( i=0; i<N ; i++ ) {
	for(j=0;j<N;j++){
	fscanf(fp,"%d ", &r_matrix[i][j]);
	
	}


}


/*compare the two matrixs*/

for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		
		if(matrix[i][j]!=r_matrix[i][j]){
		fprintf(stdout,"error on entries %d %d, the two values are not equal!\n", i,j);
		}
		
		}


}


fclose(fp);
if ( fp== NULL) {
fprintf(stderr,"error closing the file");
return -1;
}



    return 0;
}
