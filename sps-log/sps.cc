#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "defines.h"

using namespace std;

typedef pair <int, int> Int_Pair;

int build_array(vector<int>& a, int n)
{
  int i;
    
  srand(time(NULL));  
     
  for (i = 0; i < n; i++)
    a[i] = rand();
  
  return 0;
}

void array_swap(vector<int>& a, map<int, int>& undolog, map<int, int>& redolog, int n, int i)
{
  int temp, k1, k2;

  srand(time(NULL)+i*i);
  k1 = rand() % n;    
  k2 = rand() % n;     

  //cout << "swaps a[" << k1 << "] and a[" << k2 << "]" << endl;  
  
  //mcsim_log_begin();
  //mcsim_skip_instrs_begin();
  undolog.insert(Int_Pair(k1, a[k1]));
  undolog.insert(Int_Pair(k2, a[k2]));  
  //redolog.insert(Int_Pair(k1, a[k2]));
  //redolog.insert(Int_Pair(k2, a[k1]));
  //mcsim_skip_instrs_end();
  //mcsim_mem_fence();
  //mcsim_log_end();
  //mcsim_mem_fence();
  
  temp  = a[k1];
  a[k1] = a[k2];
  a[k2] = temp;   
  
}

void print_array(vector<int>& a, int n, ofstream& file)
{
  int i;

  for (i = 0; i < n; i++)
    file << a[i] << endl;
}

int main(int argc, char **argv)
{
  //mcsim_skip_instrs_begin();
  if (argc == 1) {
    printf("\n=========== An Array Usage ============\n");
    printf("Build an array with random integers, randomly swaps between entries\n");
    printf("./sps --count <item_count> --swaps <num of swaps>\n");    
    printf("<item_count>, default 10^6\n\n");
    return 0;
  }

  int i;
  int item_count = ITEM_COUNT, swaps = 0;

  for (i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--count", 7) == 0) {      
      item_count = atoi(argv[i+1]);
      ++i;      
    } else if (strncmp(argv[i], "--swaps", 7) == 0) {
      swaps = atoi(argv[i+1]);
      ++i;
    } else {
      printf("Invalid parameters: '%s'\n", argv[i]);
      return -1;
    }
  }   
  
  vector<int> array(item_count);
  map<int, int> undolog, redolog;

  // Initialization: build an array with random intergers
  if (build_array(array, item_count)) {
    cerr << "Fails to build an array" << endl;
    return -1;
  }
  
  //mcsim_skip_instrs_end(); 

#ifdef SPS_DEBUG
  ofstream orig;
  orig.open("orig.debug");
  print_array(array, item_count, orig);
#endif

  

  // randomly swaps between entries
  for (i = 0; i < swaps; i++) {
    array_swap(array, undolog, redolog, item_count, i); // swap two entris at a time
  }

  //cout << "done swaps " << i << endl;

  // make sure log structures are not dummy, will not discard by compile+O3
  //mcsim_skip_instrs_begin();
  //cout << "dummy: undolog.size= " << undolog.size() << endl;
  //cout << "dummy: redolog.size= " << redolog.size() << endl;
  //mcsim_skip_instrs_end();
  
#ifdef SPS_DEBUG
  ofstream now;
  now.open("now.debug");
  print_array(array, item_count, now);
#endif
  
  return 0;
}
