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

using namespace std;

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

// Build a B-tree with random keys and values
double bt_put(btree_impl* pbt, unsigned long srand_seed, int value_size, int item_count, char* rand_v)
{      
  double put_time = 0;
  int i, num, r = 0;
  struct timespec put_ts1, put_ts2;  
  
  // srand_seed is fixed number 
  // so every time should build a same tree
  srand(srand_seed);  
  
  char* v = (char*)malloc(sizeof(char*) * value_size);
  char *k;
  // PUT: build a B-tree at size of item_count
  for (i = 0; i < item_count; i++) {
    clock_gettime(CLOCK_REALTIME, &put_ts1);
    k = (char *)malloc(sizeof(char) * KEY_SIZE);
    if (NULL == k) {
      printf("ran out of memory allocating a key\n");
      exit(-1);
    }
  
    num = rand();
    snprintf(k, KEY_SIZE, "foo_rand_%015d", num);
    snprintf(v, value_size, "%s", rand_v);
            
    r = rand();
    v[num % (value_size - 1)] = 'a' + (r % 26);
    
    pbt->insert(k, v);
    clock_gettime(CLOCK_REALTIME, &put_ts2);
    put_time += diff_clocktime(&put_ts1, &put_ts2);
  }
  
  return put_time;
}

// Search for a random key, remove it if find it, insert it otherwise
void* bt_get(void *thrarg)
{
  thread_arg_t *arg;
  arg = (thread_arg_t *) thrarg;

  double get_time = 0;
  int num, r = 0;
  struct timespec get_ts1, get_ts2, insert_ts1, insert_ts2, del_ts1, del_ts2;
  int vsize = arg->value_size;

  unsigned long seed;

  // User control of can find or cannot find the key, method1
  if (arg->can_find == 0)
    seed = pthread_self();
  else
    seed = arg->srand_seed;
  //printf("debug3: seed=%lu\n", seed);
  
  srand(seed);
  char* k = (char *)malloc(sizeof(char) * KEY_SIZE);
  if (NULL == k) {
    printf("ran out of memory allocating a key\n");
    exit(-1);
  }
  char* v = (char*)malloc(sizeof(char) * vsize);
      
  num = rand();  
  snprintf(k, KEY_SIZE, "foo_rand_%015d", num);
  
  // User control of can find or cannot find the key, method2
  /*if (arg->can_find == 0)
    snprintf(k, KEY_SIZE, "%025d", num);    
  else 
  snprintf(k, KEY_SIZE, "foo_rand_%015d", num);*/

  //printf("debug1: k=%s\n", k);
  
  clock_gettime(CLOCK_REALTIME, &get_ts1);
  
  //lock a mutex prior to read the shared data
  pthread_rdwr_rlock_np(arg->pmutex);
  v = arg->pbt->find(k).data();
  pthread_rdwr_runlock_np(arg->pmutex);   

  clock_gettime(CLOCK_REALTIME, &get_ts2);
  get_time += diff_clocktime(&get_ts1, &get_ts2);
  
  printf("debug2: k=%s\n", k);
  
  if (v == NULL) { 
    printf("key not found, insert it: %s\n", k);    
    clock_gettime(CLOCK_REALTIME, &insert_ts1);
        
    char* insert_v = (char*)malloc(sizeof(char) * vsize);
    
    snprintf(insert_v, vsize, "%s", arg->rand_v);        
    r = rand();
    insert_v[num % (vsize - 1)] = 'a' + (r % 26);

    //lock a mutex prior to updating the shared data 
    pthread_rdwr_wlock_np(arg->pmutex); 
    arg->pbt->insert(k, insert_v);
    pthread_rdwr_wunlock_np(arg->pmutex);

    clock_gettime(CLOCK_REALTIME, &insert_ts2);
    get_time += diff_clocktime(&insert_ts1, &insert_ts2);
  } else {
    printf("key found, remove it: %s\n", k);
    clock_gettime(CLOCK_REALTIME, &del_ts1);
    
    //lock a mutex prior to updating the shared values 
    pthread_rdwr_wlock_np(arg->pmutex); 
    arg->pbt->erase(k);
    pthread_rdwr_wunlock_np(arg->pmutex);

    clock_gettime(CLOCK_REALTIME, &del_ts2);
    get_time += diff_clocktime(&del_ts1, &del_ts2);        
  }

#ifdef BTREE_DEBUG
  printf("debug: get_time = %f, max_get_time = %f\n", get_time, arg->max_get_time);
#endif
  
  if (get_time > arg->max_get_time)
    arg->max_get_time = get_time;
    
  pthread_exit((void*) 0);
}
