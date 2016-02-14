#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/unordered_map.hpp>

#include "defines.h"

using namespace std;

typedef pair <int, int> Int_Pair;

int build_hash_table(unordered_map <int, int> & hm, int item_count)
{
  int i, v;
    
  srand(time(NULL));  
     
  for (i = 1; i <= item_count; ++i) {    
    v = rand();
    hm.insert(Int_Pair (i, v));
  }
  
  return 0;
}

void print_hash_table(unordered_map<int, int>& hm, std::ofstream& file)
{
  unordered_map<int, int> :: iterator hmIter;
  
  for (hmIter = hm.begin(); hmIter != hm.end(); hmIter ++) {
    file << "< " << hmIter->first << ", " << hmIter->second
	 << " >" << std::endl;
  }    
}

int main(int argc, char **argv)
{
  int i; 
  int item_count = ITEM_COUNT;   

  for (i = 1; i != argc; ++i) {
    if (strncmp(argv[i], "--count", 7) == 0) {
      item_count = atoi(argv[i+1]);
      ++i;    
    } else {
      printf("Invalid parameters: '%s'\n", argv[i]);
      return -1;
    }
  }   
  
  unordered_map <int, int> hm;
  const unordered_map <int, int>* hm_pointer = &hm;
  
  // Initialization: build a hash table with random interger pairs
  if (build_hash_table(hm, item_count)) {
    std::cerr << "Fails to build a hash table" << std::endl;
    return -1;
  }

  std::ofstream ofs("hashtable.bin", std::ios::binary);
  
  boost::archive::binary_oarchive archive(ofs);
  archive << (*hm_pointer);

#ifdef DEBUG
  std::ofstream orig;
  orig.open("hashtable.debug");
  print_hash_table(hm, orig);
#endif

  return 0;
}
