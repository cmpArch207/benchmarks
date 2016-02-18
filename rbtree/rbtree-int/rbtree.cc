#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>

#include "defines.h"

using namespace std;

typedef pair< int, int > int_pair;
map< int, int > rbtree;

#ifdef PERSISTENT
#ifdef REDOLOG
vector<int_pair> redo_log;
#endif
#ifdef UNDOLOG
vector<int_pair> undo_log;
#endif
#endif

void rbtree_insert( int arr_size )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  map< int, int >::iterator rbtree_iter;
  int key   = rand() % arr_size;
  int value = rand();
  // Checks if the rbtree contains the key
  rbtree_iter = rbtree.find( key );
  mcsim_skip_instrs_end();
  
  // If not, insert it the key+value into the table
  if ( rbtree_iter != rbtree.end() )
  {
    #ifdef PERSISTENT
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( int_pair( key, value ) );
    #endif
    #ifdef UNDOLOG
    undo_log.push_back( int_pair( key, rbtree_iter->second ) );
    #endif

    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif
    
    rbtree.insert( int_pair( key, value ) );
  }
  else
  {
    #ifdef PERSISTENT
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( int_pair( key, value ) );
    #endif
    #ifdef UNDOLOG
    undo_log.push_back( int_pair( key, value ) );
    #endif

    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif
    
    rbtree.insert( int_pair( key, value ) );
  }
}

void rbtree_delete( int arr_size )
{
  mcsim_skip_instrs_begin();
  srand( time( NULL ) );  
  map< int, int >::iterator rbtree_iter;
  int key = rand() % arr_size;
  // Checks if the rbtree contains the key
  rbtree_iter = rbtree.find( key );
  mcsim_skip_instrs_end();
  
  // If so, delete the key+value pair from the rbtree
  if ( rbtree_iter != rbtree.end() )
  {
    #ifdef PERSISTENT
    mcsim_log_begin();

    #ifdef REDOLOG
    redo_log.push_back( int_pair( key, rbtree_iter->second ) );
    #endif
    #ifdef UNDOLOG
    undo_log.push_back( int_pair( key, rbtree_iter->second ) );
    #endif
    
    mcsim_mem_fence();
    mcsim_log_end();
    mcsim_mem_fence();
    #endif

    rbtree.erase( rbtree_iter );
  }
}

void rbtree_initialize( int arr_size )
{
  srand( time( NULL ) );  

  map< int, int >::iterator rbtree_iter;
  for ( int i = 0; i < arr_size; i++ )
  {
    int key = i;
    // Checks if the rbtree contains the key
    rbtree_iter = rbtree.find( key );
    // If not, insert it the key+value into the table
    if ( rbtree_iter == rbtree.end() )
    {
      int value = rand();
      rbtree.insert( int_pair( key, value ) );
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

  rbtree_initialize( arr_size );
  mcsim_skip_instrs_end();
 
  for ( int i = 0; i < num_iterations; i++ )
  {
    int random = rand() % 100 + 1;
    if ( random > PROB )
      rbtree_insert( arr_size );
    else
      rbtree_delete( arr_size );
  }

  return 0;
}

