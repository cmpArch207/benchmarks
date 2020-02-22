//various tx size

/*#include <stdio.h>*/
/*#include <stdlib.h>*/
#include <iostream>
#include <emmintrin.h>
#include <time.h>
/*#include <unistd.h>*/
/*#include <valgrind/callgrind.h>*/

#include "helper.h"
#include "defines.h"

#include "System.hpp"

using namespace std;

/*global variables*/
int workload = 1 << 23;
int array_size = workload >> 2; //# of elts of this workload
int tx_size = 128 >> 2; 		//# of elts in ea tx
int * Array;
int * log;


void warmup(int array_size) {
	Array = (int *)malloc(array_size * sizeof(int));
	log = (int *)malloc(2 * array_size * sizeof(int));
	int i;
	int n, p;
	n = rand();
	p = rand();
	for (i = 0; i < array_size; ++i) {
		log[2 * i] = n;
		log[2 * i + 1] = p;
		asm volatile("sfence");

		Array[i] = n;
	}
}

void run_seq_redo(int array_size) { //run it sequantially, redo log
	//must call warmup first
	uint64_t log_start, log_end, log_cycles = 0, min_log_cycles = uint64_t(-1), 
	tot_start, tot_end, tot_cycles = 0, min_tot_cycles = uint64_t(-1);
	int i, j, k, times;
	int n, p;
	//int elt_size = 1;
	//repeat multiple times
	for (times = 0; times < 100; ++times) {
		n = rand();
		p = rand();
		tot_start = rdtsc();
		for (i = 0; i < array_size / tx_size; ++i) {
//			log_start = rdtsc();

			//System::profile("log", [&]() {
			for (j = 0; j < tx_size; ++j)
			{
				_mm_stream_si32(&log[2 * i * tx_size + j + 0], n);
				_mm_stream_si32(&log[2 * i * tx_size + j + 1], p);
			}
			asm volatile("sfence");
			//});
	//			log_end = rdtsc();
	//			log_cycles = log_end - log_start;
	//			if (log_cycles < min_log_cycles)
	//				min_log_cycles = log_cycles;

			for (j = 0; j < tx_size; ++j)
			{
				Array[i * tx_size + j] = n;
			}
		}
		tot_end = rdtsc();
		tot_cycles = tot_end - tot_start;
		if (tot_cycles < min_tot_cycles)
			min_tot_cycles = tot_cycles;
	}

//	printf("bypass: log_cycles = %lu \n", min_log_cycles);
	printf("bypass: tot_cycles = %lu \n", min_tot_cycles);
}



int main() {
	srand(time(NULL));

	//warmup
	warmup(array_size);

	//CALLGRIND_START_INSTRUMENTATION;
	run_seq_redo(array_size);
	//CALLGRIND_STOP_INSTRUMENTATION;
	//CALLGRIND_DUMP_STATS;


	return 0;
}



		



