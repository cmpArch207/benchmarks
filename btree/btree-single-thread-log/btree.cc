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

#include "btree.h"
#include "defines.h"

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
  //mcsim_skip_instrs_begin();
  if (argc == 1) {
    printf("\n=========== A Simple B-tree Usage ============\n");
    printf("Search for an items in a Btree, \n");
    printf("remove it if found, insert it if not found\n\n");
    printf("./btree --get <num of iterations> --count <btree scale> --size <value size>\n");    
    printf("btree scale: item count, default 10^6\n");
    printf("value size: size of each item, default 2048\n");
    return 0;
  }

  int i, bench = GET_BENCH;
  int value_size = VALUE_SIZE, item_count = ITEM_COUNT;
  int total_iterations = TOTAL_ITERATIONS;
  unsigned long srand_seed = 0;
  double put_time = 0, get_time = 0;

  for (i = 1; i != argc; ++i) {
    if (strncmp(argv[i], "--get", 5) == 0) {
      bench = GET_BENCH;
      total_iterations = atoi(argv[i+1]);
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

  btree_impl bt;   
  map< char*, char* > undolog, redolog;

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
  ofstream orig;
  orig.open ("orig.debug");
  bt.print(orig); 
  debugfile1.close();
#endif

  //mcsim_skip_instrs_end();

  // **************************************************************
  // GET: search for a key, if present then remove it, otherwise insert it
  int j;
  for (j = 0; j != (total_iterations/2); ++j) {// can find, remove              
    get_time += bt_get(&bt, undolog, redolog, srand_seed, value_size, rand_v, 1, j);  
  }

  for (j = 0; j != (total_iterations/2); ++j) {// cannot find, insert              
    get_time += bt_get(&bt, undolog, redolog, srand_seed, value_size, rand_v, 0, j);  
  }
  
  printf("GET %d  time: %0.5f us ", item_count, (double)get_time / 1000.0);
  printf("Throughput: %0.2f\n", item_count / ((double)get_time / (double)1000000000L));  

  // make sure log structures are not dummy, will not discard by compile+O3
  //mcsim_skip_instrs_begin();
  cout << "dummy: undolog.size= " << undolog.size() << endl;
  cout << "dummy: redolog.size= " << redolog.size() << endl;
  //mcsim_skip_instrs_end();
  

#ifdef BTREE_DEBUG
  ofstream now;
  now.open ("now.debug");
  bt.print(now); 
  now.close();
#endif
  
  free(rand_v);
  return 0;
}
