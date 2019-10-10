#include <stdio.h>
#include <stdlib.h>

#define COUNTER 5
#define COLORS 7
#define SPREAD 0.05
static void *write_attractors(void * arggs);

static float * roots;
static int ** lookup;
static float ** convergences; //TODO: should be float?
int n_row_col;
int main()
{
  //color lookup table -- implement in main code
  lookup = malloc(sizeof(int*) * COLORS);
  for (size_t ix = 0; ix < COLORS; ++ix){
    lookup[ix] = malloc(sizeof(int)*3);
  }
  //Black
  lookup[0][0] = 0;
  lookup[0][1] = 0;
  lookup[0][2] = 0;
  //White
  lookup[1][0] = 255;
  lookup[1][1] = 255;
  lookup[1][2] = 255;
  //Blue
  lookup[2][0] = 0;
  lookup[2][1] = 0;
  lookup[2][2] = 255;
  //Red
  lookup[3][0] = 255;
  lookup[3][1] = 0;
  lookup[3][2] = 0;
  //Green
  lookup[4][0] = 0;
  lookup[4][1] = 255;
  lookup[4][2] = 0;
  //Yellow
  lookup[5][0] = 255;
  lookup[5][1] = 255;
  lookup[5][2] = 0;
  //Purple
  lookup[6][0] = 255;
  lookup[6][1] = 0;
  lookup[6][2] = 255;
    
  //Test section do not copy
  roots = malloc(sizeof(float) * COUNTER);
  roots[0] = 1;
  roots[1] = 2.7453;
  roots[2] = 3;
  roots[3] = 888;
  roots[4] = 999;

  n_row_col = 1000;
  //end of test sesction
}


static void * write_attractors(void * args) {
  // TODO: File initialization
  // TODO: is it a row or a matrix access ? i dont get it :/ change loop structure accordingly
    for(size_t ix = 0; ix < n_row_col; ++ix) {
      for(size_t jx = 0; jx < n_row_col; ++jx)
	for(size_t px = 0; px < COLORS; ++px)
	  if((roots[px] <= convergences[ix][jx] + SPREAD) && (roots[px] >= convergences[ix][jx] - SPREAD))
	    // TODO: Writing color in there
	    //color is lookup[px][?] ? is for 0->R; 1->G; 2->B
	    break;
	    }
}
