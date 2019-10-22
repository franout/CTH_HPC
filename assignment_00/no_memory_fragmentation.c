#include <stdio.h>
#include <stdlib.h>
#define SIZE 128 

int main ( void ) {


int * asentries = (int*) malloc(sizeof(int) * SIZE*SIZE);
int ** as1 = (int**) malloc(sizeof(int*) * SIZE);

/*avoiding memory fragmentation */

for ( size_t ix = 0, jx = 0; ix < SIZE; ++ix, jx+=SIZE )
	  as1[ix] = asentries + jx;

for ( size_t ix = 0; ix < SIZE; ++ix )
	  for ( size_t jx = 0; jx < SIZE; ++jx )
		      as1[ix][jx] = 0;

printf("%d\n", as1[0][0]);

free(as1);
free(asentries);
return 0;
}
