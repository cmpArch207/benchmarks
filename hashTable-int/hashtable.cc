#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <unordered_map>

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
  //mcsim_skip_instrs_begin();
  if (argc == 1) {
    printf("\n=========== A Hash Table Usage ============\n");
    printf("Build a hash table with random integer pairs, search for an item, \n");
    printf("\tremove it if found, insert it otherwise\n\n");
    printf("./hashtable --get <num of iterations> --count <hashtable scale>\n");    
    printf("<hashtable scale>\tHash table item count, default 10^6\n\n");
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
  
  unordered_map <int, int> hm;  
  
  // Initialization: build a hash table with random interger pairs
  if (build_hash_table(hm, item_count)) {
    std::cerr << "Fails to build a hash table" << std::endl;
    return -1;
  }

#ifdef HM_DEBUG
  std::ofstream orig;
  orig.open("orig.debug");
  print_hash_table(hm, orig);
#endif

  //mcsim_skip_instrs_end();
  
  unordered_map <int, int> :: iterator hm_SrchIter;

  for (int k=1; k != (total_iterations +1); ++k) {
    search_key = k*2*(item_count/total_iterations)-1;
    // Search for an integer
    //mcsim_skip_instrs_begin();
    hm_SrchIter = hm.find(search_key);  
    //mcsim_skip_instrs_end();

    // Absent, insert it
    if (hm_SrchIter == hm.end()) {
      //std::cout << "Cannot find the item with a key of " << search_key 
		//<< ", insert it" << std::endl;
      // randomly generate a value
      int v;
      srand(time(NULL));
      v = rand(); 
      
      // search for a postion to insert
      //mcsim_skip_instrs_begin();      
      hm_SrchIter = hm.find(hm.size()-1);      
      //mcsim_skip_instrs_end();
      
      hm.insert(hm_SrchIter, Int_Pair(search_key, v));
    } 
    else { // Find, remove it  
      //std::cout << "Item found: <" << search_key << ", " 
		//<< hm_SrchIter->second 
		//<< ">, remove it" << std::endl;
      hm.erase(hm_SrchIter);    
    }
  }
  
#ifdef HM_DEBUG
  std::ofstream now;
  now.open("now.debug");
  //print_hash_table(hm, now);
#endif
  
  return 0;
}
