#ifndef __HELPER__
#define __HELPER__

#include <stdint.h>
//helper functions
void usage ();
void get_arg(int argc, char ** argv, int * size, int * loops, int * elt_size);
uint64_t rdtsc();

#endif
