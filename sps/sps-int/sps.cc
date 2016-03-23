#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>

#include "defines.h"

using namespace std;

typedef pair< int, int > int_pair;
vector<int> sps;

#ifdef PERSISTENT
#ifdef REDOLOG
vector<int_pair> redo_log;
#endif // REDOLOG
#ifdef UNDOLOG
vector<int_pair> undo_log;
#endif // UNDOLOG
#endif // PERSISTENT

void sps_swap( int arr_size )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  int key0   = rand() % arr_size;
  int key1   = rand() % arr_size;
  while ( key1 == key0 )
    key1 = rand() % arr_size;
  int value0 = sps[key0];
  int value1 = sps[key1];
  mcsim_skip_instrs_end();
 
  #ifdef PERSISTENT
  mcsim_log_begin();

  #ifdef REDOLOG
  redo_log.push_back( int_pair( key0, value1 ) );
  redo_log.push_back( int_pair( key1, value0 ) );
  #endif // REDOLOG
  #ifdef UNDOLOG
  undo_log.push_back( int_pair( key0, value0 ) );
  undo_log.push_back( int_pair( key1, value1 ) );
  #endif // UNDOLOG

  mcsim_mem_fence();
  mcsim_log_end();
  mcsim_mem_fence();
  #endif // PERSISTENT
  
  sps[key0] = value1;
  sps[key1] = value0;
}

void sps_initialize( int arr_size )
{
  srand( time( NULL ) );  

  for ( int key = 0; key < arr_size; key++ )
  {
    int value = rand();
    sps[key] = value;
  }
}

int main( int argc, char **argv ) 
{
  mcsim_skip_instrs_begin();
  int arr_size = ITEM_COUNT;
  int num_swaps = TOTAL_ITERATIONS;
  if ( argc == 1 ) {
    printf( "\n=========== A Simple Array Usage ============\n" );
    printf( "Search for an items in an Array, \n" );
    printf( "remove it if found, insert it if not found\n\n" );
    printf("./sps --swaps <num of swaps> --count <sps scale>\n");    
    printf("<sps scale>\tArray item count, default 10^6\n\n");
  }

  for ( int i = 1; i < argc; i++ ) 
  {
    if ( strncmp( argv[i], "--count", 7 ) == 0 ) 
    {
      arr_size = atoi( argv[i+1] );
      ++i;
    }
    else if ( strncmp( argv[i], "--swaps", 7 ) == 0 ) 
    {
      num_swaps = atoi( argv[i+1] );
      ++i;
    } 
    else 
    {
      printf( "Invalid parameters: '%s'\n", argv[i] );
      return -1;
    }
  }         

  printf( "Running benchmark with %d items and %d swaps\n",
    arr_size, num_swaps );

  sps_initialize( arr_size );
  mcsim_skip_instrs_end();
 
  for ( int i = 0; i < num_swaps; i++ )
  {
    sps_swap( arr_size );
  }

  return 0;
}

