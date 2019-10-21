
__kernel void compute_diffusion ( __global float *data,  const double  diff_c, const int rows, const int columns, int iteration_numb ){

// even if data is a matrix it is transformed into a long array -> array parallelism of gpu
int ix=row+get_global_id(0);
int jy=column+get_global_id(0);


float hl=data[ix][jy-1] ; //h(i,j-1)
float hr=data[ix][jy+1] ;  // h(i,j+1)
float ha= data[ix-1][jy]; //h(i-1,j)
float hb=data[ix+1][jy] ; // h(i+1,j)
float h=data[ix][jy];


float local_t=h+diff_c(ha+hb+hl+hr/4*h);




}


__kernel void compute_average(__global float * data, const int rows, const int columns ,__global float * avg) {



}

__kernel void compute_matrix_abs_val(__global  float * data, const int rows,const int columns,const float * average ) {


}
