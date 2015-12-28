#ifndef _DEFINES_H_
#define _DEFINES_H_


#define KEY_SIZE 25
#define VALUE_SIZE 2048

#define GET_BENCH 0
#define PUT_BENCH 1
#define DEL_BENCH 2

#define RAND_SEED 21312309
#define ITEM_COUNT 1000000
#define TOTAL_ITERATIONS 1000


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

#endif // _DEFINE_H_
