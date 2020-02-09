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
	int elt_size = 1;
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
			_mm_stream_si32(&log[2 * i + 0], n);
			_mm_stream_si32(&log[2 * i + 1], p);
		//	_mm_stream_si32(&log[2 * i + 2], n);
		//	_mm_stream_si32(&log[2 * i + 3], p);
		//	_mm_stream_si32(&log[2 * i + 4], n);
		//	_mm_stream_si32(&log[2 * i + 5], p);
		//	_mm_stream_si32(&log[2 * i + 6], n);
		//	_mm_stream_si32(&log[2 * i + 7], p);
		//	_mm_stream_si32(&log[2 * i + 8], n);
		//	_mm_stream_si32(&log[2 * i + 9], p);
		//	_mm_stream_si32(&log[2 * i + 10], n);
		//	_mm_stream_si32(&log[2 * i + 11], p);
		//	_mm_stream_si32(&log[2 * i + 12], n);
		//	_mm_stream_si32(&log[2 * i + 13], p);
		//	_mm_stream_si32(&log[2 * i + 14], n);
		//	_mm_stream_si32(&log[2 * i + 15], p);
			asm volatile("sfence");
			log_end = rdtsc();
			log_cycles = log_end - log_start;
			if (log_cycles < min_log_cycles)
				min_log_cycles = log_cycles;

			array[i * elt_size + 0] = n;
		//	array[i * elt_size + 1] = n;
		//	array[i * elt_size + 2] = n;
		//	array[i * elt_size + 3] = n;
		//	array[i * elt_size + 4] = n;
		//	array[i * elt_size + 5] = n;
		//	array[i * elt_size + 6] = n;
		//	array[i * elt_size + 7] = n;
		}
		tot_end = rdtsc();
		tot_cycles = tot_end - tot_start;
		if (tot_cycles < min_tot_cycles)
			min_tot_cycles = tot_cycles;
	}

	printf("bypass: log_cycles = %lu \n", min_log_cycles);
	printf("bypass: tot_cycles = %lu \n", min_tot_cycles);
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



		



