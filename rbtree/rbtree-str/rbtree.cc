#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>

#include "defines.h"

using namespace std;

typedef pair< string, string > str_pair;
map< string, string > rbtree;

#ifdef PERSISTENT
#ifdef REDOLOG
vector<str_pair> redo_log;
#endif // REDOLOG
#ifdef UNDOLOG
vector<str_pair> undo_log;
#endif // UNDOLOG
#endif // PERSISTENT

string random_string() {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

  // Adjusts length of string to be randomly between length / 2 and length
  int length = ( rand() % 64 ) + 64; // Generates a string length between 16 and 32 characters
  string str( length, 0 );
  for ( int i = 0; i < length; i++ ) 
  {
    str[i] = alphanum[ rand() % ( sizeof( alphanum ) - 1 ) ];
  }

  return str;
}

void rbtree_insert( int arr_size, int num_iterations )
{
  mcsim_skip_instrs_begin();
  //srand( time( NULL ) );  
  map< string, string >::iterator rbtree_iter;
  string key   = to_string( rand() % arr_size );
  string value = random_string();
  // Checks if the rbtree contains the key
  rbtree_iter = rbtree.find( key );
  mcsim_skip_instrs_end();
  
  // If not, insert it the key+value into the table
  if ( rbtree_iter != rbtree.end() )
  {
    #ifdef PERSISTENT
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( str_pair( key, value ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( str_pair( key, rbtree_iter->second ) );
    #endif // UNDOLOG

    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // PERSISTENT
    
    rbtree.insert( str_pair( key, value ) );
  }
  else
  {
    #ifdef PERSISTENT
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
    
    rbtree.insert( str_pair( key, value ) );
  }
}

void rbtree_delete( int arr_size, int num_iterations )
{
  mcsim_skip_instrs_begin();
  //srand( time( NULL ) );  
  map< string, string >::iterator rbtree_iter;
  string key = to_string( rand() % arr_size );
  // Checks if the rbtree contains the key
  rbtree_iter = rbtree.find( key );
  mcsim_skip_instrs_end();
  
  // If so, delete the key+value pair from the rbtree
  if ( rbtree_iter != rbtree.end() )
  {
    #ifdef PERSISTENT
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( str_pair( key, rbtree_iter->second ) );
    #endif // REDOLOG
    #ifdef UNDOLOG
    undo_log.push_back( str_pair( key, rbtree_iter->second ) );
    #endif // UNDOLOG
    
    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif // PERSISTENT

    rbtree.erase( rbtree_iter );
  }
}

void rbtree_initialize( int arr_size, int num_iterations )
{
  //srand( time( NULL ) );  

  map< string, string >::iterator rbtree_iter;
  for ( int i = 0; i < arr_size; i++ )
  {
    string key = to_string( i );
    // Checks if the rbtree contains the key
    rbtree_iter = rbtree.find( key );
    // If not, insert it the key+value into the table
    if ( rbtree_iter == rbtree.end() )
    {
      string value = random_string();
      rbtree.insert( str_pair( key, value ) );
    }
  }
}

int main( int argc, char **argv ) 
{
  mcsim_skip_instrs_begin();
  int arr_size = ITEM_COUNT;
  int num_iterations = TOTAL_ITERATIONS;
  if ( argc == 1 ) {
    printf( "\n=========== A Simple RB Tree Usage ============\n" );
    printf( "Search for an items in a RB Tree, \n" );
    printf( "remove it if found, insert it if not found\n\n" );
    printf("./rbtree --get <num of iterations> --count <rbtree scale>\n");    
    printf("<rbtree scale>\tRB Tree item count, default 10^6\n\n");
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

  rbtree_initialize( arr_size, num_iterations );
  mcsim_skip_instrs_end();
 
  for ( int i = 0; i < num_iterations; i++ )
  {
    int random = rand() % 100 + 1;
    if ( random > PROB )
      rbtree_insert( arr_size, num_iterations );
    else
      rbtree_delete( arr_size, num_iterations );
  }

  return 0;
}

