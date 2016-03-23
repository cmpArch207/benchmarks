// SixDegs: based on Kevin bacon game
// functions are search, insert, and remove (type ./sixdegs for details)
// 7.2012

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <string.h> 
#include <fstream>
#include <vector>
#include <map>
#include <boost/config.hpp>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/lexical_cast.hpp>

#include "defines.h"

using namespace std;
using namespace boost;

typedef pair < int, pair<string, string> > Complex_Pair;


template <typename DistanceMap>
class bacon_number_recorder : public default_bfs_visitor {
public:
  bacon_number_recorder(DistanceMap dist) : d(dist) { }

  template <typename Edge, typename Graph>
  void tree_edge(Edge e, const Graph& g) const
  {
    typename graph_traits<Graph>::vertex_descriptor
      u = source(e, g), v = target(e, g);
    d[v] = d[u] + 1;
  }
private:
  DistanceMap d;
};

// Convenience function
template <typename DistanceMap>
bacon_number_recorder<DistanceMap>
record_bacon_number(DistanceMap d)
{
  return bacon_number_recorder < DistanceMap > (d);
}

int main(int argc, char **argv)
{
  mcsim_skip_instrs_begin();
  if (argc == 1) {
    printf("\n=========== An Six Degree Usage ============\n");
    printf("Build a scale-free graph, apply to actions to the graph: \n");
    printf("(1) Search for a path of length no more than six between two vertices\n");
    printf("(2) Modify the graph by inserting and removing edges\n");
    printf("./sixdegs --search <num of iterations>\n");
  }

  int i, bench = SEARCH_BENCH, total_iterations = TOTAL_ITERATIONS;
  int base = BASE, diff = DIFF;
  string src_node, other_node;

  for (i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--search", 8) == 0) {      
      bench = SEARCH_BENCH; 
      total_iterations = atoi(argv[i+1]);
      ++i;
    } else {
      printf("Invalid parameters: '%s'\n", argv[i]);
      return -1;
    }
  }   
   
  printf( "Running benchmark with %d iterations\n" total_iterations );

  ifstream datafile("input.dat");
  if (!datafile) {
    cerr << "No input.dat file" << endl;
    return -1;
  }
  
  typedef adjacency_list <vecS, vecS, undirectedS, property <vertex_name_t, string>, property <edge_name_t, string> > Graph;
  Graph g;
  
  typedef property_map < Graph, vertex_name_t >::type actor_name_map_t;
  actor_name_map_t actor_name = get(vertex_name, g);
  typedef property_map < Graph, edge_name_t >::type movie_name_map_t;
  movie_name_map_t connecting_movie = get(edge_name, g);

  typedef graph_traits < Graph >::vertex_descriptor Vertex;
  typedef map < string, Vertex > NameVertexMap;
  NameVertexMap actors;
   
  // Initialization: create a graph from the input file
  for (string line; getline(datafile, line);) {
    char_delimiters_separator <char>sep(false, "", ";");
    tokenizer <> line_toks(line, sep);
    tokenizer <>::iterator it = line_toks.begin();
    string actors_name = *it++;
    NameVertexMap::iterator pos;
    bool inserted;
    Vertex u, v;
    tie(pos, inserted) = actors.insert(make_pair(actors_name, Vertex()));
    if (inserted) {
      u = add_vertex(g);
      actor_name[u] = actors_name;
      pos->second = u;
    } else
      u = pos->second;

    string movie_name = *it++;
    
    tie(pos, inserted) = actors.insert(make_pair(*it, Vertex()));
    if (inserted) {
      v = add_vertex(g);
      actor_name[v] = *it;
      pos->second = v;
    } else
      v = pos->second;
    
    graph_traits < Graph >::edge_descriptor e;
    tie(e, inserted) = add_edge(u, v, g);
    if (inserted)
      connecting_movie[e] = movie_name;
    
  } // Initialization done

  #ifdef PERSISTENT
  #ifdef UNDOLOG
  map< int, pair<string, string> > undolog;
  #endif // UNDOLOG
  #ifdef REDOLOG
  map< int, pair<string, string> > redolog;
  #endif // REDOLOG
  #endif // PERSISTENT

  mcsim_skip_instrs_end();
  
  // Search for a path of length no more than six between two vertices
  if (bench == SEARCH_BENCH) {
    cout << "num_edges before search: " << num_edges(g) << endl;
    string movie_name;
    Vertex u, v;
    bool inserted;
    for (i=0; i != total_iterations; ++i) {                              

      // insert      

      mcsim_skip_instrs_begin();
      src_node = lexical_cast<string>(i+base);            
      other_node = lexical_cast<string>(i+base + diff-1);      

      movie_name = src_node + "_with_" + other_node;
      u = actors[src_node];
      v = actors[other_node];      
      graph_traits < Graph >::edge_descriptor e;   
      pair<string, string> entry(src_node, other_node);
      mcsim_skip_instrs_end();
   
      #ifdef PERSISTENT 
      mcsim_log_begin();      
      //mcsim_skip_instrs_begin();
      #ifdef UNDOLOG
      undolog.insert(Complex_Pair(0, entry)); // 0= not connected
      #endif // UNDOLOG
      #ifdef REDOLOG
      redolog.insert(Complex_Pair(1, entry)); // 1= connected
      #endif // REDOLOG
      // mcsim_skip_instrs_end();
      mcsim_mem_fence();
      mcsim_log_end();
      mcsim_mem_fence();
      #endif // PERSISTENT

      tie(e, inserted) = add_edge(u, v, g);
      if (inserted) {
	connecting_movie[e] = movie_name;
	cout << src_node << " and " << other_node
	     << " now connected." << endl;
      }
                                  
      // remove
      mcsim_skip_instrs_end();
      src_node = lexical_cast<string>(i+base);      
      other_node = lexical_cast<string>(i+ diff + base/2);
      
      u = actors[src_node];
      v = actors[other_node];
      entry = make_pair(src_node, other_node);
      mcsim_skip_instrs_end();
      
      #ifdef PERSISTENT 
      mcsim_log_begin();      
      //mcsim_skip_instrs_begin();
      #ifdef UNDOLOG
      undolog.insert(Complex_Pair(1, entry)); // 1= connected
      #endif // UNDOLOG
      #ifdef REDOLOG
      redolog.insert(Complex_Pair(0, entry)); // 0= not connected
      #endif // REDOLOG
      // mcsim_skip_instrs_end();
      mcsim_mem_fence();
      mcsim_log_end();
      mcsim_mem_fence();
      #endif // PERSISTENT

      remove_edge(u, v, g);
      
      cout << src_node << " and " << other_node
	   << " now disconnected." << endl;              
    }
    cout << "num_edges after searches: " << num_edges(g) << endl;
  }
  
  // make sure log structures are not dummy, will not discard by compile+O3
  #ifdef PERSISTENT 
  mcsim_skip_instrs_begin();
  #ifdef UNDOLOG
  cout << "dummy: undolog.size= " << undolog.size() << endl;
  #endif // UNDOLOG
  #ifdef REDOLOG
  cout << "dummy: redolog.size= " << redolog.size() << endl;
  #endif // REDOLOG
  mcsim_skip_instrs_end();
  #endif // PERSISTENT
      
  return 0;
}
