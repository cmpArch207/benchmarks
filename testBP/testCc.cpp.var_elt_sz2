//various elt size

/*#include <stdio.h>*/
/*#include <stdlib.h>*/
#include <iostream>
#include <emmintrin.h>
#include <time.h>
/*#include <unistd.h>*/
/*#include <valgrind/callgrind.h>*/

#include "helper.h"
#include "defines.h"

using namespace std;

/*global variables*/
int array_size = 1 << 21;
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
	int elt_size = 8;
	//repeat multiple times
	for (j = 0; j < 100; ++j) {
		n = rand();
		p = rand();
		tot_start = rdtsc();
		for (i = 0; i < array_size / elt_size; ++i) {
			log_start = rdtsc();

//			_mm_stream_si32(&log[2 * i * elt_size + k], n);
//			_mm_stream_si32(&log[2 * i * elt_size + k + 1], p);
//			asm volatile("mfence");

//        mcsim_log_begin();
			log[2 * i * elt_size + 0] = n;
			log[2 * i * elt_size + 1] = p;
			log[2 * i * elt_size + 2] = n;
			log[2 * i * elt_size + 3] = p;
			log[2 * i * elt_size + 4] = n;
			log[2 * i * elt_size + 5] = p;
			log[2 * i * elt_size + 6] = n;
			log[2 * i * elt_size + 7] = p;
			log[2 * i * elt_size + 8] = n;
			log[2 * i * elt_size + 9] = p;
			log[2 * i * elt_size + 10] = n;
			log[2 * i * elt_size + 11] = p;
			log[2 * i * elt_size + 12] = n;
			log[2 * i * elt_size + 13] = p;
			log[2 * i * elt_size + 14] = n;
			log[2 * i * elt_size + 15] = p;
			asm volatile("sfence");
			_mm_clflush(&log[2 * i * elt_size + 0]);
			_mm_clflush(&log[2 * i * elt_size + 1]);
			_mm_clflush(&log[2 * i * elt_size + 2]);
			_mm_clflush(&log[2 * i * elt_size + 3]);
			_mm_clflush(&log[2 * i * elt_size + 4]);
			_mm_clflush(&log[2 * i * elt_size + 5]);
			_mm_clflush(&log[2 * i * elt_size + 6]);
			_mm_clflush(&log[2 * i * elt_size + 7]);
			_mm_clflush(&log[2 * i * elt_size + 8]);
			_mm_clflush(&log[2 * i * elt_size + 9]);
			_mm_clflush(&log[2 * i * elt_size + 10]);
			_mm_clflush(&log[2 * i * elt_size + 11]);
			_mm_clflush(&log[2 * i * elt_size + 12]);
			_mm_clflush(&log[2 * i * elt_size + 13]);
			_mm_clflush(&log[2 * i * elt_size + 14]);
			_mm_clflush(&log[2 * i * elt_size + 15]);
			asm volatile("sfence");
			log_end = rdtsc();
			log_cycles = log_end - log_start;
			if (log_cycles < min_log_cycles)
				min_log_cycles = log_cycles;

			array[i * elt_size + 0] = n;
			array[i * elt_size + 1] = n;
			array[i * elt_size + 2] = n;
			array[i * elt_size + 3] = n;
			array[i * elt_size + 4] = n;
			array[i * elt_size + 5] = n;
			array[i * elt_size + 6] = n;
			array[i * elt_size + 7] = n;
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



		



