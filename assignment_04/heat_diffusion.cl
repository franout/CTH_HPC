dimension identified by dimindx. The global work-item
ID specifies the work-item ID based on the number of
global work-items specified to execute the kernel.*/
/*
get_globa_id uniquely identifies ech work item executing the kernel
get_local_id() uniquely identifies each work item in a work group

*/

const int ix = get_global_id(0)+1;
const int jy= get_local_id(0)+1;

float hl=data[ix*columns + jy-1] ; //h(i,j-1)
float hr=data[ix*columns+jy+1] ;  // h(i,j+1)
float ha= data[(ix-1)*columns+jy]; //h(i-1,j)
float hb=data[(ix+1)*columns+jy] ; // h(i+1,j)
float h=data[ix*columns + jy];


printf("%d %d %d %d %f %f %f %f %f\n",get_global_size(0),get_local_size(0),ix,jy,hl,hr,ha,hb,h);

float local_t=h+diff_c*(ha+hb+hl+hr/4*h);
data[ix*columns+jy]=local_t;

}


__kernel void compute_average(__global float * data, const int rows, const int columns ,__global * scratch,__global float * avg) {

int gsz = get_global_size(0);
int gix = get_global_id(0);
int lsz = get_local_size(0);
int lix = get_local_id(0);
float acc = 0;
for (int cix = gix ; cix < rows*columsn ; cix+=gsz) {
	 acc += data[cix];
	 }
	 scratch[lix] = acc;
	 barrier(CLK_LOCAL_MEM_FENCE);

	 for(int offset = lsz/2; offset>0; offset/=2) {
		 if (lix < offset)
		 scratch[lix] += scratch[lix+offset];
		 barrier(CLK_LOCAL_MEM_FENCE);
		 }

		 if (lix == 0) {
		 avg[get_group_id(0)] = scratch[0];

		 }
		 }

		 __kernel void compute_matrix_abs_val(__global  float * data, const int rows,const int columns,__global const float * average ) {

float old_data= data [] ;

data [] = fabs(old_data - average);


		 }
