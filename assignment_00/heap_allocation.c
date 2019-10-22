#include <stdlib.h>
#include <stdio.h>
#define SIZE 10

int main ( void ) {


int *as = (int *) malloc ( sizeof(int)*SIZE) ;

for(size_t ix=0; ix< SIZE; ++ix ) {

as[ix]=0;
}

printf("%d\n",as[0]);
free(as);
return 0;

}

