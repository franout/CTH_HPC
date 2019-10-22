
__kernel void compute_diffusion ( __global float *data, const float diff_c, const int rows, const int columns, int iteration_numb ){

// even if data is a matrix it is transformed into a long array -> array parallelism of gpu
/*Returns the unique global work-item ID value for
dimension identified by dimindx. The global work-item
ID specifies the work-item ID based on the number of
global work-items specified to execute the kernel.*/


const int ix = get_global_id(0);
const int jy= get_global_id(1);
//printf("%d %d %d\n",id,get_global_id(0),get_num_groups(0));
float hl=data[ix*columns + jy-1] ; //h(i,j-1)
float hr=data[ix*columns+jy+1] ;  // h(i,j+1)
float ha= data[(ix-1)*columns+jy]; //h(i-1,j)
float hb=data[(ix+1)*columns+jy] ; // h(i+1,j)
float h=data[ix*columns + jy];


//printf("%d %d %f %f %f %f %f\n",ix,jy,hl,hr,ha,hb,h);

float local_t=h+diff_c*(ha+hb+hl+hr/4*h);
data[ix*columns+jy]=local_t;

}


__kernel void compute_average(__global float * data, const int rows, const int columns ,__global float * avg) {



}

__kernel void compute_matrix_abs_val(__global  float * data, const int rows,const int columns,__global const float * average ) {


}
