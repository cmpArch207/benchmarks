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

#include "btree.h"
#include "defines.h"

long diff_clocktime(struct timespec *time1, struct timespec *time2)
{
  struct timespec result;

  /* We assume time1 < time2 */
  result.tv_sec = time2->tv_sec - time1->tv_sec;
  if (time2->tv_nsec < time1->tv_nsec) {
    result.tv_nsec = time2->tv_nsec + 1000000000L - time1->tv_nsec ;
    result.tv_sec-- ;       /* Borrow a second. */
  } else {
    result.tv_nsec = time2->tv_nsec - time1->tv_nsec ;
  }

  return result.tv_sec*1000000000L + result.tv_nsec;
}

double bt_put(btree_impl* pbt, 
	      unsigned long srand_seed, int value_size, 
	      int item_count, char* rand_v)
{      
  double put_time = 0;
  int i, num, r = 0;
  struct timespec put_ts1, put_ts2;
  
  
  srand(srand_seed);  
  
  char* v = (char*)malloc(sizeof(char*) * value_size);
  char *k;
  // PUT: build a B-tree with random characters
  for (i = 0; i != item_count; ++i) {
    clock_gettime(CLOCK_REALTIME, &put_ts1);
    k = (char *)malloc(sizeof(char) * KEY_SIZE);
    if (NULL == k) {
      printf("ran out of memory allocating a key\n");
      return 1;
    }
  
    //num = rand();
    num = srand_seed + i;
    snprintf(k, KEY_SIZE, "foo_rand_%015d", num);
    snprintf(v, value_size, "%s", rand_v);

    //printf("debug1: %s, %s\n", k, v);
    
    r = rand();
    v[num % (value_size - 1)] = 'a' + (r % 26);
    //printf("debug2: %s\n", v);
    pbt->insert(k, v);

    clock_gettime(CLOCK_REALTIME, &put_ts2);
    put_time += diff_clocktime(&put_ts1, &put_ts2);
  }  
  return put_time;
}

double bt_get(btree_impl* pbt, 
	      map< char*, char* >& undolog, 
	      map< char*, char* >& redolog,
	      unsigned long srand_seed, int value_size, 
	      char* rand_v, int can_find, int index)
{
  //mcsim_skip_instrs_begin();
  double get_time = 0;
  int num, r = 0;
  struct timespec get_ts1, get_ts2, insert_ts1, insert_ts2, del_ts1, del_ts2;
  
  srand(srand_seed);

  char* k = (char *)malloc(sizeof(char) * KEY_SIZE);
  if (k == NULL) {
    printf("ran out of memory allocating a key\n");
    return 1;
  }
  char* v = (char*)malloc(sizeof(char) * value_size);
        
  //num = rand();
  num = srand_seed + index;  
  
  if (can_find == 0)   
    snprintf(k, KEY_SIZE, "%d", num); // make it cannot find 
  else
    snprintf(k, KEY_SIZE, "foo_rand_%015d", num); // make it can find
 
  //r = rand(); // Consume random number    
  clock_gettime(CLOCK_REALTIME, &get_ts1);
  v = pbt->find(k).data();  
  clock_gettime(CLOCK_REALTIME, &get_ts2);
  get_time += diff_clocktime(&get_ts1, &get_ts2);
  
  //mcsim_skip_instrs_end();

  if (v == NULL) { 
    printf("key not found, insert it: %s\n", k);    

    //mcsim_skip_instrs_begin();
    clock_gettime(CLOCK_REALTIME, &insert_ts1);
        
    char* insert_v = (char*)malloc(sizeof(char) * value_size);
    
    snprintf(insert_v, value_size, "%s", rand_v);        
    r = rand();
    insert_v[num % (value_size - 1)] = 'a' + (r % 26);
    //mcsim_skip_instrs_end();
    
    //mcsim_log_begin();
    undolog.insert(Char_Pair(k, insert_v));
    //mcsim_skip_instrs_begin();
    //redolog.insert(Char_Pair(k, insert_v));
    //mcsim_skip_instrs_end();
    //mcsim_mem_fence();
    //mcsim_log_end();
    //mcsim_mem_fence();
    
    pbt->insert(k, insert_v);

    clock_gettime(CLOCK_REALTIME, &insert_ts2);
    get_time += diff_clocktime(&insert_ts1, &insert_ts2);
  } else {
    printf("key found, remove it: %s\n", k);        
    clock_gettime(CLOCK_REALTIME, &del_ts1);

    //mcsim_log_begin();
    undolog.insert(Char_Pair(k, v));
    //mcsim_skip_instrs_begin();
    //redolog.insert(Char_Pair(k, v));
    //mcsim_skip_instrs_end();
    //mcsim_mem_fence();
    //mcsim_log_end();
    //mcsim_mem_fence();

    pbt->erase(k);
    
    clock_gettime(CLOCK_REALTIME, &del_ts2);
    get_time += diff_clocktime(&del_ts1, &del_ts2);
  }    
  return get_time;
}
