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
int array_size = 2048;
int * array;
int * log;
/*int * array = (int *)malloc(array_size * sizeof(int));*/
/*int * log = (int *)malloc(2 * array_size * sizeof(int));*/


void warmup(int array_size) {
	array = (int *)malloc(array_size * sizeof(int));
	log = (int *)malloc(2 * array_size * sizeof(int));
	int n, p;
	int i;
	for (i = 0; i < array_size; ++i) {
		n = rand();
		p = rand();


		//_mm_stream_si32(&log[2 * i], n);
		//_mm_stream_si32(&log[2 * i + 1], p);
		//asm volatile("mfence");
		mcsim_log_begin();
		log[2 * i] = n;
		log[2 * i + 1] = p;
		
		mcsim_mem_fence();
		mcsim_log_end();
		mcsim_mem_fence();

		array[i] = n;
	}
}

void run(int array_size) {
	//must call warmup first
  uint64_t start, end;
	start = rdtsc();
	int i, j;
	int n, p;
	//repeat 5 times
	for (j = 0; j < 5; ++j) {
		for (i = 0; i < array_size; ++i) {
			n = rand();
			p = rand();

			//_mm_stream_si32(&log[2 * i], n);
			//_mm_stream_si32(&log[2 * i + 1], p);
			//asm volatile("mfence");

			mcsim_log_begin();
			log[2 * i] = n;
			log[2 * i + 1] = p;
			
			mcsim_mem_fence();
			mcsim_log_end();
			mcsim_mem_fence();
			array[i] = n;
		}
	}
	end = rdtsc();
	uint64_t tot_cycles = end - start;


	/*double thp = (double)(5 * array_size) / (double)tot_cycles;*/
	/*printf("bypass: throughput = %.5lf \n", thp);*/
	printf("bypass: cycles = %lu \n", tot_cycles);

}



int main() {
	srand(time(NULL));

	//warmup
	warmup(array_size);

	//run
	//CALLGRIND_START_INSTRUMENTATION;
	run(array_size);
	//CALLGRIND_STOP_INSTRUMENTATION;
	//CALLGRIND_DUMP_STATS;


	return 0;
}



		



