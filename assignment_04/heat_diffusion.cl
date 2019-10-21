__kernel void dot_prod_mul(
	     __global const float * a,
	     __global const float * b,
	     __global float * c
	     )
{


/*kernel is afunction or a task to do Saved as a separate file (typically ending in .cl) and loaded on runtime */


int ix = get_global_id(0);
c[ix] = a[ix] * b[ix];
}
