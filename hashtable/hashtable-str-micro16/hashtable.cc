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

typedef pair< string, string > str_pair;
unordered_map< string, string > hashtable;

#ifdef PERSISTENT
#ifdef REDOLOG
vector<str_pair> redo_log;
#endif // REDOLOG
#ifdef UNDOLOG
vector<str_pair> undo_log;
#endif // UNDOLOG
#endif // PERSISTENT

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

void hash_insert( int arr_size, int num_iterations )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  unordered_map< string, string >::iterator hash_iter;
  string key   = to_string( rand() % arr_size );
  string value = random_string( num_iterations );
  // Checks if the hashtable contains the key
  hash_iter = hashtable.find( key );
  mcsim_skip_instrs_end();
  
  // If not, insert it the key+value into the table
  if ( hash_iter != hashtable.end() )
  {
    #ifdef PERSISTENT
    mcsim_tx_begin();
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( str_pair( key, value ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( str_pair( key, hash_iter->second ) );
    #endif // UNDOLOG

    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // PERSISTENT
    
    hashtable.insert( str_pair( key, value ) );
    #ifdef PERSISTENT
    mcsim_tx_end();
    mcsim_clwb( &( hashtable[key] ) );
    #endif // PERSISTENT
  }
  else
  {
    #ifdef PERSISTENT
    mcsim_tx_begin();
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( str_pair( key, value ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( str_pair( key, value ) );
    #endif // UNDOLOG

    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // PERSISTENT
    
    hashtable.insert( str_pair( key, value ) );
    #ifdef PERSISTENT
    mcsim_tx_end();
    mcsim_clwb( &( hashtable[key] ) );
    #endif // PERSISTENT
  }
}

void hash_delete( int arr_size, int num_iterations )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  unordered_map< string, string >::iterator hash_iter;
  string key = to_string( rand() % arr_size );
  // Checks if the hashtable contains the key
  hash_iter = hashtable.find( key );
  mcsim_skip_instrs_end();
  
  // If so, delete the key+value pair from the hashtable
  if ( hash_iter != hashtable.end() )
  {
    #ifdef PERSISTENT
    mcsim_tx_begin();
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( str_pair( key, hash_iter->second ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( str_pair( key, hash_iter->second ) );
    #endif // UNDOLOG
    
    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // PERSISTENT

    hashtable.erase( hash_iter );
    #ifdef PERSISTENT
    mcsim_tx_end();
    #endif // PERSISTENT
  }
}

void hash_initialize( int arr_size, int num_iterations )
{
  srand( time( NULL ) );  

  unordered_map< string, string >::iterator hash_iter;
  for ( int i = 0; i < arr_size; i++ )
  {
    string key = to_string( i );
    // Checks if the hashtable contains the key
    hash_iter = hashtable.find( key );
    // If not, insert it the key+value into the table
    if ( hash_iter == hashtable.end() )
    {
      string value = random_string( num_iterations );
      hashtable.insert( str_pair( key, value ) );
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
    else if ( strncmp( argv[i], "--ops", 5 ) == 0 ) 
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

  hash_initialize( arr_size, num_iterations );
  mcsim_skip_instrs_end();
 
  for ( int i = 0; i < num_iterations; i++ )
  {
    int random = rand() % 100 + 1;
    if ( random > PROB )
      hash_insert( arr_size, num_iterations );
    else
      hash_delete( arr_size, num_iterations );
  }

  return 0;
}

