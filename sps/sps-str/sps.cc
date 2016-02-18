#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>

#include "defines.h"

using namespace std;

typedef pair< int, string > keyval_pair;
vector<string> sps_vector;

#ifdef PERSISTENT
#ifdef REDOLOG
vector<keyval_pair> redo_log;
#endif
#ifdef UNDOLOG
vector<keyval_pair> undo_log;
#endif
#endif

string random_string( size_t length ) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

  string str( length, 0 );
  for ( int i = 0; i < length; i++ ) 
  {
    str[i] = alphanum[ rand() % ( sizeof( alphanum ) - 1 ) ];
  }

  return str;
}

void sps_swap( int arr_size )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  int key0   = rand() % arr_size;
  int key1   = rand() % arr_size;
  while ( key1 == key0 )
    key1 = rand() % arr_size;
  string value0 = sps_vector[key0];
  string value1 = sps_vector[key1];
  mcsim_skip_instrs_end();
 
  #ifdef PERSISTENT
  mcsim_log_begin();

  #ifdef REDOLOG
  redo_log.push_back( keyval_pair( key0, value1 ) );
  redo_log.push_back( keyval_pair( key1, value0 ) );
  #endif
  #ifdef UNDOLOG
  undo_log.push_back( keyval_pair( key0, value0 ) );
  undo_log.push_back( keyval_pair( key1, value1 ) );
  #endif

  mcsim_mem_fence();
  mcsim_log_end();
  mcsim_mem_fence();
  #endif
  
  sps_vector[key0] = value1;
  sps_vector[key1] = value0;
}

void sps_initialize( int arr_size, int num_swaps )
{
  srand( time( NULL ) );  

  for ( int i = 0; i < arr_size; i++ )
  {
    string value = random_string( num_swaps );
    sps_vector.push_back( value );
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

  sps_initialize( arr_size, num_swaps );
  mcsim_skip_instrs_end();
 
  for ( int i = 0; i < num_swaps; i++ )
  {
    sps_swap( arr_size );
  }

  return 0;
}

