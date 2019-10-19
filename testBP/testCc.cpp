#include <iostream>
/*#include <stdio.h>*/
/*#include <stdlib.h>*/
#include <emmintrin.h>
#include <time.h>
/*#include <unistd.h>*/
/*#include <valgrind/callgrind.h>*/

#include "helper.h"
#include "defines.h"

/*global variables*/
int array_size = 8192;
int * array;
int * log;


void warmup(int array_size) {
	array = (int *)malloc(array_size * sizeof(int));
	log = (int *)malloc(2 * array_size * sizeof(int));
//	int i;
//	int n, p, n1;
//	for (i = 0; i < array_size; ++i) {
//		n = rand();
//		p = rand();
//
//		//n1 = rand();
//
//		log[2 * i] = n;
//		log[2 * i + 1] = p;
//		asm volatile("mfence");
//
////		mcsim_log_begin();
//		//log[2 * i] = n;
//		//log[2 * i + 1] = p;
//		
////		mcsim_mem_fence();
////		mcsim_log_end();
////		mcsim_mem_fence();
//
//		array[i] = n;
//	}
}

void run(int array_size) {
	//must call warmup first
  uint64_t start, end, tot_cycles = 0, min_cycles = uint64_t(-1);
	int i, j;
	int n, p;
	//repeat 5 times
	for (j = 0; j < 1; ++j) {
    //start = rdtsc();
		for (i = 0; i < array_size; ++i) {
			n = rand();
			p = rand();

			log[2 * i] = n;
			log[2 * i + 1] = p;
			asm volatile("mfence");

//			mcsim_log_begin();
			//log[2 * i] = n;
			//log[2 * i + 1] = p;
			
//			mcsim_mem_fence();
//			mcsim_log_end();
//			mcsim_mem_fence();

			array[i] = n;
		}
    //end = rdtsc();
    //tot_cycles = end - start;
    //if (tot_cycles < min_cycles)
    //  min_cycles = tot_cycles;
	}

	//printf("cached: cycles = %lu \n", tot_cycles);

}


int main() {
	srand(time(NULL));

	//warmup
	warmup(array_size);


	//CALLGRIND_START_INSTRUMENTATION;
	run(array_size);
	//CALLGRIND_STOP_INSTRUMENTATION;
	//CALLGRIND_DUMP_STATS;

	return 0;
}
