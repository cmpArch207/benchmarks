// Courtesy of Onur Mutlu

#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(int argc, char **argv) {

  int size = atoi(argv[1]);
  int element_size = atoi(argv[2]);
  int traversals = atoi(argv[3]);
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
    index_array[i] = (rand()/(((double)RAND_MAX + 1) / (size))) * element_size;
  }

  start_time = time(NULL);
  for (i=0; i < traversals; i++) {
    for (j=0; j < size; j+=8) {
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

}

