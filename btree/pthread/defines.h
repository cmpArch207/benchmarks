#ifndef _DEFINES_H_
#define _DEFINES_H_

//#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "btree.h"

#define KEY_SIZE 25
#define VALUE_SIZE 2048
#define ITEM_COUNT 1000000

#define GET_BENCH 0
#define PUT_BENCH 1
#define DEL_BENCH 2
#define RAND_SEED 21312309

#define NUM_THREADS 4

using namespace std;

long diff_clocktime(struct timespec *time1, struct timespec *time2);

template <int _innerslots, int _leafslots>
struct btree_traits_speed {
        static const bool   selfverify = false;
        static const bool   debug = false;

        static const int    leafslots = _innerslots;
        static const int    innerslots = _leafslots;
};

static char kMaxStr[] = "__nv_";

struct key_less {
  bool operator() (char* key1, char* key2) const {
    // NULL is lesser than all keys 
    if (key2 == NULL)
      return false;
    if (key1 == NULL)
      return true;
    
    int cmp = strcmp(key1, key2);
    // if a == b, return false
    if (cmp == 0)
      return false;
    if (strcmp(key2, kMaxStr) == 0) {
      // if key2 equals max key return true
      return true;
    } else if (strcmp(key1, kMaxStr) == 0) {
      // if key1 equals max key return false 
      return false;
    }
    // return true if key1 < key2
    return (cmp < 0);
  }
};

typedef stx::btree<char*, char*, std::pair<char*, char*> , 
  struct key_less, struct btree_traits_speed<32,32>, false> btree_impl;

// pthread mutex object, readers/writer 
typedef struct rdwr_var {
  int readers_reading;
  int writer_writing;
  pthread_mutex_t mutex;
  pthread_cond_t lock_free;
} pthread_rdwr_t;

typedef void * pthread_rdwrattr_t;

// pthread mutex functions
int pthread_rdwr_init_np(pthread_rdwr_t *rdwrp, pthread_rdwrattr_t *attrp);
int pthread_rdwr_rlock_np(pthread_rdwr_t *rdwrp);
int pthread_rdwr_runlock_np(pthread_rdwr_t *rdwrp);
int pthread_rdwr_wlock_np(pthread_rdwr_t *rdwrp);
int pthread_rdwr_wunlock_np(pthread_rdwr_t *rdwrp);
int pthread_rdwr_destroy_np(pthread_rdwr_t *rdwrp);

// Argument list of pthread utility function bt_get()
typedef struct thr_arg {  
  btree_impl* pbt;
  unsigned long srand_seed;
  int value_size;
  char* rand_v;
  int can_find;
  pthread_rdwr_t *pmutex;
  double max_get_time;
} thread_arg_t;

// build a B-tree
double bt_put(btree_impl* pbt, unsigned long srand_seed, int value_size, int item_count, char* rand_v);

void* bt_get(void* thrarg);

#endif // _DEFINE_H_
