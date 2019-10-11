#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#define n_row_col 1000





int main(){

  double **convergences  = (double**) malloc(sizeof(double*) * n_row_col);

  for (int ix = 0, jx = 0; ix < n_row_col; ++ix){

    double *convergence = (double*) malloc(sizeof(double) * n_row_col);
    for(int iy =0; iy < n_row_col; ++iy){
      convergence[iy] = iy*iy;

    }

    convergences[ix] = convergence;

  }



  //printf("%10f\n",convergences[10][10]);



  int maxElement = 1;

  // checking each element of matrix
  // if it is greater than maxElement,
  // update maxElement
  for (size_t i = 0; i < n_row_col; i++) {
    for (size_t j = 0; j < n_row_col; j++) {
      if (convergences[i][j] > maxElement) {
	maxElement = convergences[i][j];
      }
    }
  }


  maxElement--;

  double mt = 255.0/maxElement;
  for (size_t i = 0; i < n_row_col; i++) {
    for (size_t j = 0; j < n_row_col; j++) {

      convergences[i][j] = floor(mt*(convergences[i][j]-1));
    }}



  printf("max: %d",maxElement);

  for(size_t i = 0; i<n_row_col; i++)
    {

      printf("max: %10f", convergences[10][i]);

    }


  for (size_t i=0; i<n_row_col; ++i) {
    free(convergences[i]);
  }
  free(convergences);



}
