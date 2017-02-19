
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include <string>
#include <unistd.h>

#include "sync.h"
#include "defines.h"

const string prog = "stm";

// helper functions
void usage() {
  cout << "Usage: ./execbin [-s array_size] [-l loops] [-e element_size].\n";
  exit(EXIT_FAILURE);
}

void get_arg(int argc, char ** argv, int * size, int * loops, int *elt_sz) {
  if (argc != 5 and argc != 7) 
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
        *elt_sz = atoi(optarg);
        break;
      default:
        usage();
    }
  }
}

int main(int argc, char **argv) {

  //int size = atoi(argv[1]);
  //int element_size = atoi(argv[2]);
  //int traversals = atoi(argv[3]);

  //while (!is_started()) {
  ////cout << "waiting\n";
  //;
  //}

  //get parameters
  int size, traversals, element_size = 1;
  if (argc != 5 and argc != 7)
    usage();
  get_arg(argc, argv, &size, &traversals, &element_size);

  int i, j;
  time_t start_time;
  time_t end_time;

  int *array = NULL;
  int *target_array = NULL;
  int *index_array = NULL;
  long long int sum = 0;

  printf("size=%d, element_size=%d, traversals=%d sizeof(int):%d\n", size, element_size, traversals, sizeof(int));
  printf("Array size in bytes: %lld\n", (long long int)size*element_size*sizeof(int)); 

  array = (int *)malloc(size*element_size*sizeof(int));
  target_array = (int *)malloc(size*element_size*sizeof(int));
  index_array = (int *)malloc(size*sizeof(int));

  for (i=0; i < size; i++) {
    array[i*element_size] = (rand()/(((double)RAND_MAX + 1) / (size))) * element_size;
    index_array[i] = i * element_size;
  }

  start_time = time(NULL);
  for (i=0; i < traversals; i++) {
    for (j=0; j + 8 < size; j+=8) {
      listen(i*size+j, prog);
      //printf("indices: %d %d %d %d %d %d %d %d\n", index_array[j], index_array[j+1], index_array[j+2], index_array[j+3], index_array[j+4], index_array[j+5], index_array[j+6], index_array[j+7]);

      target_array[index_array[j]] = array[index_array[j]];
      target_array[index_array[j+1]] = array[index_array[j+1]];
      target_array[index_array[j+2]] = array[index_array[j+2]];
      target_array[index_array[j+3]] = array[index_array[j+3]];
      target_array[index_array[j+4]] = array[index_array[j+4]];
      target_array[index_array[j+5]] = array[index_array[j+5]];
      target_array[index_array[j+6]] = array[index_array[j+6]];
      target_array[index_array[j+7]] = array[index_array[j+7]];
    }
  }
  end_time = time(NULL);

  printf("Time diff: %lld\n", (long long int)end_time-(long long int)start_time);
  printf("Sum: %lld\n", sum);

  term_prog(i*size+j, prog);
}
