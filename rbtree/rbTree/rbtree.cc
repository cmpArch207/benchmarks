#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <map>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>

#include "defines.h"

using namespace std;

typedef pair <int, int> Int_Pair;

void print_rbtree(map<int, int>& rt, ofstream& file)
{
  map<int, int> :: iterator rtIter;
  
  for (rtIter = rt.begin(); rtIter != rt.end(); rtIter ++) {
    file << "< " << rtIter->first << ", " << rtIter->second
	 << " >" << endl;
  }    
}

int main(int argc, char **argv)
{
  mcsim_skip_instrs_begin();
  if (argc == 1) {
    printf("\n=========== An RBTree Usage ============\n");
    printf("Build an RBtree with random integer pairs, search for an item, \n");
    printf("\tremove it if found, insert it otherwise\n\n");
    printf("./rbtree --get <num of iterations> --count <rbtree scale>\n");
    printf("<rbtree scale>\tRBTree item count, default 10^6\n\n");
    return 0;
  }

  int i; 
  int item_count = ITEM_COUNT;  
  int search_key = -1;
  int total_iterations = TOTAL_ITERATIONS;

  for (i = 1; i != argc; ++i) {
    if (strncmp(argv[i], "--get", 5) == 0) {      
      total_iterations = atoi(argv[i+1]);
      ++i;      
    } else if (strncmp(argv[i], "--count", 7) == 0) {
      item_count = atoi(argv[i+1]);
      if (item_count < total_iterations) {
	printf("Error: Item_count < Total_iterations\n");
	return -1;
      }
      ++i;    
    } else {
      printf("Invalid parameters: '%s'\n", argv[i]);
      return -1;
    }
  }   
  
  map <int, int> rt;  
    
  // Initialization: build an RBTree with random interger pairs
  ifstream ifs("rbtree.bin", ios::binary);
  boost::archive::binary_iarchive inarch(ifs);
  inarch >> rt;

#ifdef RBTREE_DEBUG
  ofstream orig;
  orig.open("orig.debug");
  print_rbtree(rt, orig);
#endif

  mcsim_skip_instrs_end();
  
  map <int, int> :: iterator rt_SrchIter;

  for (int k = 1; k != (total_iterations + 1); ++k) {
    search_key = k*2*(item_count/total_iterations) -1;
    // Search for an integer
    mcsim_skip_instrs_begin();
    rt_SrchIter = rt.find(search_key);  
    mcsim_skip_instrs_end();

    // Absent, insert it
    if (rt_SrchIter == rt.end()) {
      cout << "Cannot find the item with a key of " << search_key 
	   << ", insert it" << endl;
      // randomly generate a value
      int v;
      srand(time(NULL));
      v = rand();    
      
      mcsim_skip_instrs_begin();
      -- rt_SrchIter; // move rt_SrchIter to one position before rt.end()
      mcsim_skip_instrs_end();
      
      rt.insert(rt_SrchIter, Int_Pair(search_key, v));
    } 
    else { // Find, remove it  
      cout << "Item found: <" << search_key << ", " 
	   << rt_SrchIter->second 
	   << ">, remove it" << endl;
      rt.erase(rt_SrchIter);    
    }
  }
  
#ifdef RBTREE_DEBUG
  ofstream now;
  now.open("now.debug");
  print_rbtree(rt, now);
#endif
  
  return 0;
}
