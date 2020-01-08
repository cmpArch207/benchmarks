#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <string.h> /* for memcmp */
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

#include <pthread.h>

#include "btree.h"
#include "defines.h"

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

using namespace std;

char *rand_str(char *dst, int size) 
{
  static const char text[] = "abcdefghijklmnopqrstuvwxyz"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i, len = size - 1;// = rand() % (size - 1);
  for ( i = 0; i < len; ++i ) {
    dst[i] = text[rand() % (sizeof text - 1)];
  }
  dst[i] = '\0';
  return dst;
}


int main(int argc, char **argv)
{    
  if (argc == 1) {
    printf("\n=========== A Simple B-tree Usage ============\n");
    printf("Build a B-tree, search for an item, \n");
    printf("\tremove it if found, insert it otherwise\n\n");
    printf("./btree --todo [p1] --count [p2] --size [p3]\n");
    printf("--todo\t\tput: build a B-tree\n");
    printf("\t\tget: search for an item\n");
    printf("p1\t\t1:     can find the item;\n\t\tother: cannot find the item\n");
    printf("p2\t\tB-tree item count, default 10^6\n");
    printf("p3\t\tValue size of each item, default 2048\n");
    return 0;
  }

  int i, bench = GET_BENCH, can_find = 1; 
  int value_size = VALUE_SIZE, item_count = ITEM_COUNT;
  unsigned long srand_seed = 0;
  double put_time = 0;

  for (i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--get", 5) == 0) {
      bench = GET_BENCH;
      can_find = atoi(argv[i+1]);
      ++i;
    } else if (strncmp(argv[i], "--put", 5) == 0) {
      bench = PUT_BENCH;    
    } else if (strncmp(argv[i], "--count", 7) == 0) {
      item_count = atoi(argv[i+1]);
      ++i;
    } else if (strncmp(argv[i], "--size", 6) == 0) {
      value_size = atoi(argv[i+1]);
      ++i;
    } else {
      printf("Invalid parameters: '%s'\n", argv[i]);
      return -1;
    }
  }       

  // Define a B-tree object
  btree_impl bt;    

  srand_seed = RAND_SEED;
  srand(srand_seed);
  char* rand_v = (char *)malloc(sizeof(char) * value_size);
  rand_str(rand_v, value_size);
  
  // PUT: build a B-tree
  put_time += bt_put(&bt, srand_seed, value_size, item_count, rand_v);  

  printf("PUT %d time: %0.5f us ", item_count, (double)put_time / 1000.0);
  printf("Throughput: %0.2f\n", item_count / ((double)put_time / (double)1000000000L));
  if (bench == PUT_BENCH) {
    return 0;
  }

#ifdef BTREE_DEBUG
  ofstream debugfile1, debugfile2;
  debugfile1.open ("1.debug");
  bt.print(debugfile1); 
  debugfile1.close();
#endif
  
  // ******************************************************************
  // GET: Search for a key, if present then remove it, otherwise insert it 
  
  // Pthread defines
  int numThreads = NUM_THREADS;
  pthread_t thr[numThreads];
  pthread_attr_t attr;
  void *status;  

  //thr = (pthread_t *)malloc(numThreads * sizeof(pthread_t *));      

  // Initialize mutex  
  pthread_rdwr_t pthread_rw_mutex;
  if (pthread_rdwr_init_np(&pthread_rw_mutex, NULL)) {
    printf("pthread_rdwr_init_np error\n");
    exit(-1);
  }  

  thread_arg_t thread_arg;
    
  // Initialize bt_get() argument list
  thread_arg.pbt = &bt;
  thread_arg.srand_seed = srand_seed;
  thread_arg.value_size = value_size;
  thread_arg.rand_v = rand_v;
  thread_arg.can_find = can_find;
  thread_arg.pmutex = &pthread_rw_mutex;  
  thread_arg.max_get_time = 0;
  
  // Create threads
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // by default  

  for (i = 0; i < numThreads; i++) {        
    if (pthread_create(&thr[i], &attr, bt_get, (void*) &thread_arg)) {
      printf("Could not create thread %d for GET\n", i);
      exit(-1);
    }           
  }    

  pthread_attr_destroy(&attr);
  
  // Exit threads
  for (i = 0; i < numThreads; i++) {
    if (pthread_join(thr[i], &status)) {
      printf("Could not join thread %d for GET\n", i);
      exit(-1);
    }    
  }
  
  if (pthread_rdwr_destroy_np(&pthread_rw_mutex)) {
    printf("pthread_rdwr_destroy_np error\n");
    exit(-1);
  }
    
  printf("GET %d  time: %0.5f us ", item_count, (double)thread_arg.max_get_time / 1000.0);
  printf("Throughput: %0.2f\n", item_count / ((double)thread_arg.max_get_time / (double)1000000000L));  

#ifdef BTREE_DEBUG
  debugfile2.open ("2.debug");
  bt.print(debugfile2); 
  debugfile2.close();
#endif
  
  free(rand_v);
  return 0;
}
