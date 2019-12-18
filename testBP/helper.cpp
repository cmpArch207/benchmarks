#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "helper.h"

// helper functions
void usage() {
        printf("Usage: rcn_frdly_* [-s size] [-l loops] [-e element size].\n");
        exit(1);
}

void get_arg(int argc, char ** argv, int * size, int * loops, int * elt_size) {
        if (argc != 5 && argc != 7) 
                usage();

        // get options
        int opt;
        while ((opt = getopt(argc, argv, "s:l:e:")) != -1) {
                switch (opt) {
                        case 'l':
                                *loops = atoi(optarg);
                                break;
                        case 's':
                                *size = atoi(optarg);
                                break;
                        case 'e':
                                *elt_size = atoi(optarg);
                                break;
                        default:
                                usage();
                }
        }
}

uint64_t rdtsc() {
    unsigned long a, d;
    asm volatile ("rdtsc" : "=a" (a), "=d" (d) : : "ebx", "ecx");
    return a | ((uint64_t)d << 32);
}
