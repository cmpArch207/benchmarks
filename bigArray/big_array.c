#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <emmintrin.h> //_mm_stream: bypass cache
//#include <fstream>
//#include <vector>
//#include <new>
//#include <array>
#include <time.h>
/*#include <valgrind/callgrind.h>*/

#include "helper.h"
#include "big_array.h"
/*#include "defines.h"*/
/*#include "sync.h"*/

//global variables
int *array;
int *data_log;
enum { array_size = 8192 }; //array_size = 8k
/*int array_size;*/
//array_size  = # of the elements
//elt_size  = # of the integers for each element
int log_size, loops, elt_size = 1;//default value
int cur_log = 0;
/*int pos = loops / 2; // the # of different positions*/
int n_pos = 100; // the # of different positions, default value
int *pos;
char * prog;
uint64_t total_cycles;

void build_array() {
    //assign values to global varibles
    n_pos = log_size;

    //get time seed
    srand(time(NULL));

    //allocate mem
    array = (int *)malloc((array_size + 2 * log_size) * elt_size * sizeof(int));
    if (array == NULL) {
            printf("not enough space!\n");
            exit(1);
    }
    //data_log store the data and its index
    data_log = array + (array_size * elt_size);
    // randomly assign values
    /*printf("elt size = %d\n", elt_size);*/
    for (int i = 0; i < array_size; ++i) {
        /*printf("i = %d\n", i);*/
        int num = rand();
        for (int j = 0; j < elt_size; ++j) {
            /*printf("j = %d\n", j);*/
            // every int in an element is identical
            array[i * elt_size + j] = num;
        }
    }

}

void rcn_frdly_cc(const size_t n) {
    //must call warmup before run this function

    /*//n is the # of different positions*/
    /*int *pos = (int *)malloc(n * sizeof(int));*/
    int p;
    bool fst_loop = true;
    size_t i, j;
    uint64_t start = 0, end;//count cycles

    for (i = 0, j = 0; i < loops ; ++i, ++j) {
        /*listen(i, prog);*/
        //in the first loop,
        //save the positions
        if (i == n) {
                fst_loop = false;
                //don't count the warmup section
                start = rdtsc();
        }

        //repeatedly access n postions
        if (j == n) j = 0;

        /*//save the positions*/
        /*if (fst_loop) {*/
            /*p = rand() % array_size;*/
            /*pos[j] = p;*/
        /*}*/
        /*else*/
        p = pos[j];

        //write the log and data
        for (int i = 0; i < elt_size; ++i) {
            //generate a random num
            int n = rand();
            //save the log
            cacheLog(n, p);

            asm volatile("sfence");
            //write the data
            array[p * elt_size + i] = n;
        }
    }
    end = rdtsc();
    total_cycles = end -start;
    fprintf(stderr, "cc : cycles = %lu\n", total_cycles);
}

void rcn_frdly_bp(const size_t n) {
    //must call warmup before run this function

    // n is the # of different positions
    /*int *pos = (int *)malloc(n * sizeof(int));*/
    int p;
    bool fst_loop = true;
    int i, j;
    uint64_t start = 0, end;//count cycles

    for (i = 0, j = 0; i < loops ; ++i, ++j) {
        /*listen(i, prog);*/
        //in the first loop,
        //save the positions
        if (i == n) {
                fst_loop = false;
                //don't count the warmup section
                start = rdtsc();
        }

        //repeatedly access n postions
        if (j == n) j = 0;

        /*//save the positions*/
        /*if (fst_loop) {*/
            /*p = rand() % array_size;*/
            /*pos[j] = p;*/
        /*}*/
        /*else*/
        p = pos[j];

        //write the log and data
        for (int i = 0; i < elt_size; ++i) {
            //generate the data
            int n = rand();
            //bypass log
            bypassLog(n, p);
            asm volatile("sfence");
            //write the data
            array[p * elt_size + i] = n;
        }
    }
    end = rdtsc();
    total_cycles = end -start;
    fprintf(stderr, "bp : cycles = %lu\n", total_cycles);
}

void rcn_frdly_fl(const size_t n) {
    // n is the # of different positions
    int *pos = (int *)malloc(n * sizeof(int));
    int p;
    bool fst_loop = true;
    int i, j;
    uint64_t start = 0, end;//count cycles

    for (i = 0, j = 0; i < loops ; ++i, ++j) {
        /*listen(i, prog);*/
        //in the first loop,
        //save the positions
        if (i == n) {
                fst_loop = false;
                //don't count the warmup section
                start = rdtsc();
        }

        //repeatedly access n postions
        if (j == n) j = 0;

        //save the positions
        if (fst_loop) {
            p = rand() % array_size;
            pos[j] = p;
        }
        else
            p = pos[j];
        //write the log and data
        for (int i = 0; i < elt_size; ++i) {
            //generate the data
            int n = rand();
            //bypass log
            flushLog(n, p);
            asm volatile("sfence");
            //write the data
            array[p * elt_size + i] = n;
        }
    }
    end = rdtsc();
    total_cycles = end -start;
    fprintf(stderr, "fl : cycles = %lu\n", total_cycles);
}


void warmup_cc(const size_t n) {
    //n is the # of different positions
    pos = (int *)malloc(n * sizeof(int));
    int p;
    bool fst_loop = true;
    size_t i, j;
    uint64_t start = 0, end;//count cycles

    for (i = 0, j = 0; i < loops ; ++i, ++j) {
        /*listen(i, prog);*/
        //in the first loop,
        //save the positions
        if (i == n) {
                fst_loop = false;
                /*//don't count the warmup section*/
                start = rdtsc();
                return;
        }

        //repeatedly access n postions
        if (j == n) j = 0;

        //save the positions
        if (fst_loop) {
            p = rand() % array_size;
            pos[j] = p;
        }
        else
            p = pos[j];

        //write the log and data
        for (int i = 0; i < elt_size; ++i) {
            //generate a random num
            int n = rand();
            //save the log
            cacheLog(n, p);
            asm volatile("sfence");
            //write the data
            array[p * elt_size + i] = n;
        }
    }
    end = rdtsc();
    total_cycles = end -start;
}

void warmup_bp(const size_t n) {
    /*// n is the # of different positions*/
    pos = (int *)malloc(n * sizeof(int));
    int p;
    bool fst_loop = true;
    int i, j;
    uint64_t start = 0, end;//count cycles

    for (i = 0, j = 0; i < loops ; ++i, ++j) {
        /*listen(i, prog);*/
        //in the first loop,
        //save the positions
        if (i == n) {
                fst_loop = false;
                //don't count the warmup section
                start = rdtsc();
                return;
        }

        //repeatedly access n postions
        if (j == n) j = 0;

        //save the positions
        if (fst_loop) {
            p = rand() % array_size;
            pos[j] = p;
        }
        else
            p = pos[j];
        //write the log and data
        for (int i = 0; i < elt_size; ++i) {
            //generate the data
            int n = rand();
            //bypass log
            bypassLog(n, p);
            asm volatile("sfence");
            //write the data
            array[p * elt_size + i] = n;
        }
    }
    end = rdtsc();
    total_cycles = end -start;
}

void streaming() {
    int i, j;
    for (i = 0, j = 0; i < loops; ++i, ++j) {
        /*listen(i, prog);*/
        if (j >= array_size) j = 0;
    }
}


void show() {
    for (size_t i = 0; i < array_size; ++i)
        printf("%d ", array[i * elt_size]);
    printf("\n");
}

void bypassLog(int n, int pos) {
    if (cur_log >= log_size) cur_log = 0;

    for (int i = 0; i < elt_size; ++i) {
        _mm_stream_si32(&data_log[cur_log++], n);
        _mm_stream_si32(&data_log[cur_log++], pos);
    }
}

void cacheLog(int n, int pos) {
    if (cur_log >= log_size) cur_log = 0;

    for (int i = 0; i < elt_size; ++i) {
        data_log[cur_log++] = n;
        data_log[cur_log++] = pos;
    }
}

void flushLog(int n, int pos) {
    if (cur_log >= log_size) {
            //flush all logs back to mem
            int i;
            for (i = 0; i < elt_size * log_size; ++i)
                    clflush((void*)&data_log[i]);
            cur_log = 0;
    }

    for (int i = 0; i < elt_size; ++i) {
        data_log[cur_log++] = n;
        data_log[cur_log++] = pos;
        /*//flush immedately*/
        /*clflush((void*)&data_log[cur_log - 2]); //flush data*/
        /*clflush((void*)&data_log[cur_log - 1]); //flush pos*/
    }

}

void clflush(void * p) {
    asm volatile("sfence;"
    "clflush (%0)" :: "r"(p));
}
