#include <iostream>
#include <emmintrin.h> //_mm_stream: bypass cache
//#include <fstream>
//#include <vector>
//#include <new>
//#include <array>

#include "helper.h"
#include "clean_cache.h"
#include "defines.h"
#include "sync.h"

using namespace std;

//global variables
int * array;
int * log;
enum { array_size = 8388608 }; //array_size = 8192k
//array_size  = # of the elements
//elt_size  = # of the integers for each element
int log_size, loops, elt_size = 1;
int cur_log = 0;
int pos = 2097152; // the # of different positions
string prog;

void build_array() {
	//get time seed
	srand(time(NULL));

	//allocate mem
	array = new int [array_size * elt_size];
	log = new int [log_size * elt_size];
	//// randomly assign values
	//for (int i = 0; i < array_size; ++i) {
		//int num = rand();
		//for (int j = 0; j < elt_size; ++j) {
			//// every int in an element is identical
			//array[i*elt_size+j] = num;
		//}
	//}

}

void rcn_frdly_cc(const size_t & n) {
	//n is the # of different positions
	int * pos = new int [n];
	int p;
	bool fst_loop = true;
	size_t i, j;

	for (i = 0, j = 0; i < loops ; ++i, ++j) {
		listen(i, prog);
		//in the first loop, 
	        //put the positions into the vector
		if (i == n) fst_loop = false;

		//repeatedly access n postions
		if (j == n) j = 0;

		if (fst_loop) {
			p = rand() % array_size;
			pos[j] = p;
		}
		else
			p = pos[j];
		for (int i = 0; i < elt_size; ++i) {
			int n = rand();
			array[p * elt_size + i] = n;
			//bypassLog(n);
			cacheLog(n);
		}
	}
}

void rcn_frdly_bp(const size_t & n) {
	//n is the # of different positions
	int * pos = new int [n];
	int p;
	bool fst_loop = true;
	int i, j;

	for (i = 0, j = 0; i < loops ; ++i, ++j) {
		listen(i, prog);
		//in the first loop, 
	        //put the positions into the vector
		if (i == n) fst_loop = false;

		//repeatedly access n postions
		if (j == n) j = 0;

		if (fst_loop) {
			p = rand() % array_size;
			pos[j] = p;
		}
		else
			p = pos[j];
		for (int i = 0; i < elt_size; ++i) {
			int n = rand();
			array[p * elt_size + i] = n;
			bypassLog(n);
			//cacheLog(n);
		}
	}
}

void streaming() {
	int i, j;
	for (i = 0, j = 0; i < loops; ++i, ++j) {
		listen(i, prog);
		if (j >= array_size) j = 0;
	}
}


void show() {
	for (size_t i = 0; i < array_size; ++i) 
		cout << array[i*elt_size] << " ";
	cout << endl;
}

void bypassLog(int n) {
	if (cur_log >= log_size) cur_log = 0;

	for (int i = 0; i < elt_size; ++i) {
		_mm_stream_si32(&log[cur_log++], n);
		asm volatile("sfence");
	}
}

void cacheLog(int n) {
	if (cur_log >= log_size) cur_log = 0;

	for (int i = 0; i < elt_size; ++i) {
		log[cur_log++] = n;
	}
}


//// helper functions
//void usage() {
        //cout << "Usage: ./execbin [-s array_size] [-l loops] [-e element_size].\n";
        //exit(EXIT_FAILURE);
//}

//void get_arg(int argc, char ** argv, int * size, int * loops, int *elt_sz) {
        //if (argc != 5 and argc != 7) 
                //usage();

        //// get options
        //int opt;
        //while ((opt = getopt(argc, argv, "s:l:e:")) != -1) {
                //switch (opt) {
                        //case 'l':
                                //*loops = atoi(optarg);
                                //break;
                        //case 's':
                                //*size = atoi(optarg);
                                //break;
			//case 'e':
				//*elt_sz = atoi(optarg);
                                //break;
                        //default:
                                //usage();
                //}
        //}
//}
