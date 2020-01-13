//various elt size


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
int array_size = 1 << 17;
int * array;
int * log;



void warmup(int array_size) {
	array = (int *)malloc(array_size * sizeof(int));
	log = (int *)malloc(2 * array_size * sizeof(int));
	int i;
	int n, p;
	n = rand();
	p = rand();
	for (i = 0; i < array_size; ++i) {


		log[2 * i] = n;
		log[2 * i + 1] = p;
		asm volatile("sfence");

//		mcsim_log_begin();
		//log[2 * i] = n;
		//log[2 * i + 1] = p;
		
//		mcsim_mem_fence();
//		mcsim_log_end();
//		mcsim_mem_fence();

		array[i] = n;
	}
}

void run(int array_size) {
	//must call warmup first
	uint64_t log_start, log_end, log_cycles = 0, min_log_cycles = uint64_t(-1), 
	tot_start, tot_end, tot_cycles = 0, min_tot_cycles = uint64_t(-1);
	int i, j, k;
	int n, p;
	//repeat multiple times
	for (j = 0; j < 100; ++j) {
		n = rand();
		p = rand();
		tot_start = rdtsc();
		for (i = 0; i < array_size; ++i) {
		//	tot_start = rdtsc();
			log_start = rdtsc();
			//n = -1;
			//p = -2;

			//if (i % 1 != 0)
			{
			  log[2 * i] = n;
			  log[2 * i + 1] = p;
//			  _mm_clflush(&log[2 * i]);
//			  _mm_clflush(&log[2 * i + 1]);
			  _mm_clflush(&log[2 * i]);
			  _mm_clflush(&log[2 * i + 1]);
			}
			asm volatile("sfence");
			log_end = rdtsc();
			log_cycles = log_end - log_start;
			if (log_cycles < min_log_cycles)
				min_log_cycles = log_cycles;

//			mcsim_log_begin();
			//log[2 * i] = n;
			//log[2 * i + 1] = p;
			
//			mcsim_mem_fence();
//			mcsim_log_end();
//			mcsim_mem_fence();

			array[i] = n;
			//tot_end = rdtsc();
			//tot_cycles = tot_end - tot_start;
			//if (tot_cycles < min_tot_cycles)
			//	min_tot_cycles = tot_cycles;
		}
		tot_end = rdtsc();
		tot_cycles = tot_end - tot_start;
		if (tot_cycles < min_tot_cycles)
			min_tot_cycles = tot_cycles;
	}

	printf("cached: log_cycles = %lu \n", min_log_cycles);
	printf("cached: tot_cycles = %lu \n", min_tot_cycles);

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
