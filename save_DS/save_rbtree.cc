#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>

#include "defines.h"

using namespace std;

typedef pair <int, int> Int_Pair;

int build_rbtree(map <int, int> & rt, int item_count)
{
  int i, v;
    
  srand(time(NULL));  
     
  for (i = 1; i <= item_count; ++i) {    
    v = rand();
    rt.insert(Int_Pair (i, v));
  }  
  
  return 0;
}

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
  
  map <int, int> rt;
  const map<int, int>* rt_pointer = &rt;
  
  // Initialization: build an RBTree with random interger pairs
  if (build_rbtree(rt, item_count)) {
    cerr << "Fails to build an RBTree" << endl;
    return -1;
  }
  
  ofstream ofs("rbtree.bin", ios::binary);
  
  boost::archive::binary_oarchive archive(ofs);
  archive << (*rt_pointer);

  ofs.close();

#ifdef DEBUG
  ofstream orig;
  orig.open("rbtree.debug");
  print_rbtree(rt, orig);
#endif

  return 0;
}
