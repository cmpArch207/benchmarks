#ifndef __CLEAN_CACHE_H__
#define __CLEAN_CACHE_H__

#include <unistd.h> // getopt

void build_array();
void streaming();
void rcn_frdly_cc(const size_t);
void rcn_frdly_bp(const size_t);
void rcn_frdly_fl(const size_t);
void warmup_cc(const size_t);
void warmup_bp(const size_t);
void show();
void bypassLog(int n, int pos);
void cacheLog(int n, int pos);
void flushLog(int n, int pos);
void clflush(void * p);


#endif
