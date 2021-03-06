//various elt size

/*#include <stdio.h>*/
/*#include <stdlib.h>*/
#include <iostream>
#include <emmintrin.h>
#include <immintrin.h>
#include <time.h>
/*#include <unistd.h>*/
/*#include <valgrind/callgrind.h>*/

#include "helper.h"
#include "defines.h"

using namespace std;

/*global variables*/
int workload = 1 << 23;
int array_size = workload >> 2; //# of elts of this workload
int tx_size = 32 >> 2; 		
int * array;
int * log;
int * pos;

static void clwb(unsigned long long addr)
{
	__asm__ volatile("clwb %0" :: "m" (*(char*)addr));
//	__asm__ volatile("clwb %0" :: "m" (addr));
}

void warmup(int array_size) {
	array = (int *)malloc(array_size * sizeof(int));
	pos = (int *)malloc(array_size * sizeof(int));
	log = (int *)malloc(2 * array_size * sizeof(int));
	int i;
	int n, p;
	n = rand();
	p = rand() % array_size;
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

		pos[i] = p;
		array[i] = n;
	}
}

void run_seq_redo(int array_size) {
	//must call warmup first
	uint64_t log_start, log_end, log_cycles = 0, min_log_cycles = uint64_t(-1), 
	tot_start, tot_end, tot_cycles = 0, min_tot_cycles = uint64_t(-1);
	int i, j, k, times;
	int n, p;
	//int elt_size = 8;
	//repeat multiple times
	for (times = 0; times < 10; ++times) {
		n = rand();
		p = rand();
		tot_start = rdtsc();
		for (i = 0; i < array_size / tx_size; ++i) {
//			log_start = rdtsc();

			for (k = 0; k < tx_size; ++k) 
			{
				log[2 * i * tx_size + k + 0] = n;
				log[2 * i * tx_size + k + 1] = p;
			}
			asm volatile("sfence");
			for (k = 0; k < tx_size; ++k) 
			{
				clwb((unsigned long long )&log[2 * i * tx_size + k + 0]);
				clwb((unsigned long long )&log[2 * i * tx_size + k + 1]);
			}

			asm volatile("sfence");
//				log_end = rdtsc();
//				log_cycles = log_end - log_start;
//				if (log_cycles < min_log_cycles)
//					min_log_cycles = log_cycles;

			for (k = 0; k < tx_size; ++k) 
			{
				array[i * tx_size + k] = n;
				clwb((unsigned long long )&array[i * tx_size + k]);
			}
			asm volatile("sfence");
		}
		tot_end = rdtsc();
		tot_cycles = tot_end - tot_start;
		if (tot_cycles < min_tot_cycles)
			min_tot_cycles = tot_cycles;
	}

//	printf("cached: log_cycles = %lu \n", min_log_cycles);
	printf("cached: tot_cycles = %lu \n", min_tot_cycles);
}

void run_rand_redo(int array_size) {
	//must call warmup first
	uint64_t log_start, log_end, log_cycles = 0, min_log_cycles = uint64_t(-1), 
	tot_start, tot_end, tot_cycles = 0, min_tot_cycles = uint64_t(-1);
	int i, j, k, times;
	int n, p;
	//int elt_size = 8;
	//repeat multiple times
	for (times = 0; times < 10; ++times) {
		n = rand();
		p = rand();
		tot_start = rdtsc();
		for (i = 0; i < array_size / tx_size; ++i) {
//			log_start = rdtsc();

			for (k = 0; k < tx_size; ++k) 
			{
				log[2 * i * tx_size + k + 0] = n;
				log[2 * i * tx_size + k + 1] = p;
			}
			asm volatile("sfence");
			for (k = 0; k < tx_size; ++k) 
			{
				clwb((unsigned long long )&log[2 * i * tx_size + k + 0]);
				clwb((unsigned long long )&log[2 * i * tx_size + k + 1]);
			}

			asm volatile("sfence");
//				log_end = rdtsc();
//				log_cycles = log_end - log_start;
//				if (log_cycles < min_log_cycles)
//					min_log_cycles = log_cycles;

			for (k = 0; k < tx_size; ++k) 
			{
				array[pos[i * tx_size + k]] = n;
				clwb((unsigned long long )&array[pos[i * tx_size + k]]);
			}
			asm volatile("sfence");
		}
		tot_end = rdtsc();
		tot_cycles = tot_end - tot_start;
		if (tot_cycles < min_tot_cycles)
			min_tot_cycles = tot_cycles;
	}

//	printf("cached: log_cycles = %lu \n", min_log_cycles);
	printf("cached: tot_cycles = %lu \n", min_tot_cycles);
}


int main() {

	int temp = 0;
	srand(time(NULL));

	//warmup
	warmup(array_size);

	//CALLGRIND_START_INSTRUMENTATION;
	//run_seq_redo(array_size);
	run_rand_redo(array_size);
	//CALLGRIND_STOP_INSTRUMENTATION;
	//CALLGRIND_DUMP_STATS;


	return 0;
}



		



