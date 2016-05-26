#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <string>
#include <unordered_map>
#include <vector>
#include <utility> // pair
#include <unistd.h> // getopt

using namespace std;

typedef pair<string, string> Str_Pair;

class HT {
	private:
		unordered_map<string, string> ht;
		size_t str_len; //length of string
		size_t loops;  // tot # of iterations
		string prog; // program's name
		Str_Pair gen_entry();
		string gen_str();
		bool is_delete();
		vector<Str_Pair> log;
	public:
		HT(const size_t &, const size_t &, 
			const size_t &, string);
		void show() const;
		void warm_up();
		void non_persistent();
		void persistent();
		
};

//helper functs
void usage();
void get_arg(int, char **, int *, int *);

#endif
