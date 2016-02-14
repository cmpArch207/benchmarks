#ifndef _DEFINES_H_
#define _DEFINES_H_

#define SEARCH_BENCH 0
#define INSERT_BENCH 1
#define REMOVE_BENCH 2
#define INSERT_REMOVE 3
#define TOTAL_ITERATIONS 100

#define BASE 1000000000
#define DIFF 3

extern "C" {
  extern void mcsim_skip_instrs_begin();
  extern void mcsim_skip_instrs_end();
  extern void mcsim_spinning_begin();
  extern void mcsim_spinning_end();
  extern void mcsim_tx_begin();
  extern void mcsim_tx_end();
  extern void mcsim_log_begin();
  extern void mcsim_log_end();
  extern void mcsim_mem_fence();
}

#endif // _DEFINE_H_
