#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>

#include "defines.h"

using namespace std;

typedef pair< int, int > int_pair;
unordered_map< int, int > hashtable;

#ifdef REDOLOG
vector<int_pair> redo_log;
#endif // REDOLOG
#ifdef UNDOLOG
vector<int_pair> undo_log;
#endif // UNDOLOG

void hash_insert( int arr_size )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  unordered_map< int, int >::iterator hash_iter;
  int key   = rand() % arr_size;
  int value = rand();
  // Checks if the hashtable contains the key
  hash_iter = hashtable.find( key );
  mcsim_skip_instrs_end();
  
  // If not, insert it the key+value into the table
  if ( hash_iter != hashtable.end() )
  {
    mcsim_tx_begin();
    #ifdef BASELINE
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( int_pair( key, value ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( int_pair( key, hash_iter->second ) );
    #endif // UNDOLOG

    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // BASELINE
    
    hashtable.insert( int_pair( key, value ) );
    mcsim_tx_end();
    #ifdef CLWB
    mcsim_clwb( &( hashtable[key] ) );
    #endif // CLWB
  }
  else
  {
    mcsim_tx_begin();
    #ifdef BASELINE
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( int_pair( key, value ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( int_pair( key, value ) );
    #endif // UNDOLOG

    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // BASELINE
    
    hashtable.insert( int_pair( key, value ) );
    mcsim_tx_end();
    #ifdef CLWB
    mcsim_clwb( &( hashtable[key] ) );
    #endif // CLWB
  }
}

void hash_delete( int arr_size )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  unordered_map< int, int >::iterator hash_iter;
  int key = rand() % arr_size;
  // Checks if the hashtable contains the key
  hash_iter = hashtable.find( key );
  mcsim_skip_instrs_end();
  
  // If so, delete the key+value pair from the hashtable
  if ( hash_iter != hashtable.end() )
  {
    mcsim_tx_begin();
    #ifdef BASELINE
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( int_pair( key, hash_iter->second ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( int_pair( key, hash_iter->second ) );
    #endif // UNDOLOG
    
    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // BASELINE

    hashtable.erase( hash_iter );
    mcsim_tx_end();
  }
}

void hash_initialize( int arr_size )
{
  srand( time( NULL ) );  

  unordered_map< int, int >::iterator hash_iter;
  for ( int i = 0; i < arr_size; i++ )
  {
    int key = i;
    // Checks if the hashtable contains the key
    hash_iter = hashtable.find( key );
    // If not, insert it the key+value into the table
    if ( hash_iter == hashtable.end() )
    {
      int value = rand();
      hashtable.insert( int_pair( key, value ) );
    }
  }
}

int main( int argc, char **argv ) 
{
  mcsim_skip_instrs_begin();
  int arr_size = ITEM_COUNT;
  int num_iterations = TOTAL_ITERATIONS;
  if ( argc == 1 ) {
    printf( "\n=========== A Simple Hash Table Usage ============\n" );
    printf( "Search for an items in a Hash Table, \n" );
    printf( "remove it if found, insert it if not found\n\n" );
    printf("./hashtable --get <num of iterations> --count <hashtable scale>\n");    
    printf("<hashtable scale>\tHash table item count, default 10^6\n\n");
  }

  for ( int i = 1; i < argc; i++ ) 
  {
    if ( strncmp( argv[i], "--count", 7 ) == 0 ) 
    {
      arr_size = atoi( argv[i+1] );
      ++i;
    }
    else if ( strncmp( argv[i], "--get", 5 ) == 0 ) 
    {
      num_iterations = atoi( argv[i+1] );
      ++i;
    } 
    else 
    {
      printf( "Invalid parameters: '%s'\n", argv[i] );
      return -1;
    }
  }         

  printf( "Running benchmark with %d items and %d iterations\n",
    arr_size, num_iterations );

  hash_initialize( arr_size );
  mcsim_skip_instrs_end();
 
  for ( int i = 0; i < num_iterations; i++ )
  {
    int random = rand() % 100 + 1;
    if ( random > PROB )
      hash_insert( arr_size );
    else
      hash_delete( arr_size );
  }

  return 0;
}

