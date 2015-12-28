#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "btree.h"


#define KEY_SIZE 25
#define VALUE_SIZE 2048

#define GET_BENCH 0
#define PUT_BENCH 1
#define DEL_BENCH 2
#define RAND_SEED 21312309
#define ITEM_COUNT 1000000
#define TOTAL_ITERATIONS 100


using namespace std;

extern "C" {
  extern void mcsim_skip_instrs_begin();
  extern void mcsim_skip_instrs_end();
  extern void mcsim_spinning_begin();
  extern void mcsim_spinning_end();
  extern void mcsim_tx_begin();
  extern void mcsim_tx_end();
  extern void mcsim_log_begin();
  extern void mcsim_log_end(); 
}

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

typedef stx::btree<char*, char*, std::pair<char*, char*> , struct key_less, struct btree_traits_speed<32,32>, false> btree_impl;

long diff_clocktime(struct timespec *time1, struct timespec *time2);

double bt_put(btree_impl* pbt, unsigned long srand_seed, int value_size, 
	      int item_count, char* rand_v); 
double bt_get(btree_impl* pbt, unsigned long srand_seed, int value_size, 
	      char* rand_v, int can_find, int index);

#endif // _DEFINE_H_
