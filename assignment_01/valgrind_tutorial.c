#include <stdio.h>
#include <stdlib.h>

int main (void) {

int * as = (int*)malloc(10*sizeof(int));
int sum = 0;

for ( int ix = 0; ix < 10; ++ix ) {
  as[ix] = ix;
}
  for ( int ix = 0; ix < 10; ++ix ) {
    sum += as[ix];
  }

free(as);
return 0;
}
