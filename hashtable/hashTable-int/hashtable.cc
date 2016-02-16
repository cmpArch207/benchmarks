#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "defines.h"

#define TOTAL 100
#define INSERT 90

using namespace std;

typedef pair <int, int> Int_Pair;


int build_hash_table( unordered_map <int, int>& hm, int item_count )
{
  srand( time( NULL ) );  

  for ( int i = 1; i <= item_count; i++ ) 
  {    
    int v = rand();
    hm.insert( Int_Pair( i, v ) );
  }

  return 0;
}

void print_hash_table( unordered_map<int, int>& hm, std::ofstream& file )
{
  unordered_map<int, int>::iterator hmIter;

  for ( hmIter = hm.begin(); hmIter != hm.end(); hmIter++ ) 
  {
    file << "< " << hmIter->first << ", " << hmIter->second
      << " >" << std::endl;
  }    
}

int main( int argc, char **argv )
{
  mcsim_skip_instrs_begin();
  if ( argc == 1 ) {
    printf("\n=========== A Hash Table Usage ============\n");
    printf("Build a hash table with random integer pairs, search for an item, \n");
    printf("\tremove it if found, insert it otherwise\n\n");
    printf("./hashtable --get <num of iterations> --count <hashtable scale>\n");    
    printf("<hashtable scale>\tHash table item count, default 10^6\n\n");
  }

  //cout << "Insert = " << INSERT << " Delete = " << DELETE << endl;

  int item_count = ITEM_COUNT;  
  int total_iterations = TOTAL_ITERATIONS;

  for ( int i = 1; i != argc; i++ ) 
  {
    if ( strncmp(argv[i], "--get", 5 ) == 0 ) 
    {      
      total_iterations = atoi(argv[i+1]);
      ++i;      
    } 
    else if (strncmp(argv[i], "--count", 7) == 0) 
    {
      item_count = atoi(argv[i+1]);
      if (item_count < total_iterations) {
        printf("Error: Item_count < Total_iterations\n");
        return -1;
      }
      i++;    
    } 
    else 
    {
      printf("Invalid parameters: '%s'\n", argv[i]);
      return -1;
    }
  }   

  unordered_map <int, int> hm; 

#ifdef PERSISTENT
#ifdef UNDOLOG
  vector<Int_Pair> undolog;
#endif
#ifdef REDOLOG
  vector<Int_Pair> redolog;
#endif
#endif

  // Initialization: build a hash table with random interger pairs
  if ( build_hash_table( hm, item_count ) ) 
  {
    std::cerr << "Fails to build a hash table" << std::endl;
    return -1;
  }

#ifdef HM_DEBUG
  std::ofstream orig;
  orig.open( "orig.debug" );
  print_hash_table( hm, orig );
#endif

  mcsim_skip_instrs_end();

  unordered_map<int, int>::iterator hm_SrchIter;

  srand( time( NULL ) );
  int search_key = -1;

  for ( int k = 1; k != ( total_iterations + 1 ); k++ ) 
  {
    search_key = k * 2 * ( item_count / total_iterations ) - 1;
    // Search for an integer
    mcsim_skip_instrs_begin();
    hm_SrchIter = hm.find( search_key );  
    mcsim_skip_instrs_end();

    // Absent, insert it
    if ( hm_SrchIter == hm.end() ) 
    {
      mcsim_skip_instrs_begin();      
      std::cout << "Cannot find the item with a key of " << search_key 
        << ", insert it" << std::endl;
      // randomly generate a value
      mcsim_skip_instrs_end();     
      srand( time( NULL ) );
      int v = rand();      

      // search for a place to insert
      mcsim_skip_instrs_begin();      
      hm_SrchIter = hm.find( hm.size() - 1 );            
      mcsim_skip_instrs_end();     

      mcsim_log_begin();
      if ( hm_SrchIter != hm.end() )
      {
        #ifdef UNDOLOG 
        undolog.push_back( Int_Pair( hm_SrchIter->first, hm_SrchIter->second ) );
        #endif
        #ifdef REDOLOG
        redolog.push_back( Int_Pair( search_key, v ) );
        #endif
      }
      mcsim_mem_fence();
      mcsim_log_end();
      mcsim_mem_fence();

      hm.insert( hm_SrchIter, Int_Pair( search_key, v ) );      
    } 
    else 
    { // Find, remove it  
      mcsim_skip_instrs_begin();      
      std::cout << "Item found: <" << search_key << ", " 
        << hm_SrchIter->second 
        << ">, remove it" << std::endl;
      mcsim_skip_instrs_end();     


      #ifdef PERSISTENT
      mcsim_log_begin();
      #ifdef UNDOLOG 
      undolog.push_back( Int_Pair( search_key, hm_SrchIter->second ) );
      #endif
      #ifdef REDOLOG
      redolog.push_back( Int_Pair( search_key, hm_SrchIter->second ) );
      #endif
      mcsim_mem_fence();
      mcsim_log_end();
      mcsim_mem_fence();
      #endif

      hm.erase( hm_SrchIter );    
    }
  }

#ifdef HM_DEBUG
  std::ofstream now;
  now.open( "now.debug" );
  print_hash_table( hm, now );
#endif

  return 0;
}
