
__kernel void compute_diffusion ( __global float **data, const int row, const int columns, const double  diff_c ,  ){


/*kernel is afunction or a task to do Saved as a separate file (typically ending in .cl) and loaded on runtime */
int ix=row+get_global_id(0);
int jy=column+get_global_id(0);


float hl=data[ix][jy-1] ; //h(i,j-1)
float hr=data[ix][jy+1] ;  // h(i,j+1)
float ha= data[ix-1][jy]; //h(i-1,j)
float hb=data[ix+1][jy] ; // h(i+1,j)
float h=data[ix][jy];


float local_t=h+diff_c(ha+hb+hl+hr/4*h);




}
