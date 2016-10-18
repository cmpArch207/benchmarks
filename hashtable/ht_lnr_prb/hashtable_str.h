#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <string>
#include <unordered_map>
#include <vector>
#include <utility> // pair
#include <unistd.h> // getopt

using namespace std;


int * build_ht();
void fill_ht();
int gen_num();
int find_pos(int);
void insert_ent(int, int, int);
void delete_ent(int);
void show();
bool is_delete();
void non_persistent();
void persistent();

//helper functs
void usage();
void get_arg(int, char **, int *, int *, int *);

#endif
