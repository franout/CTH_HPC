#include "./mylib.h"
#include <stdio.h>
#include <stdlib.h>

extern int  N;


void mul_cpx_separatefile( double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im  ) {

*a_re= ( (*b_re)  *(*c_re) - (*c_im)*(*b_im) );
*a_im= ((* c_im)*(* b_re)+(* c_re)*(*b_im) );


return ;
}
