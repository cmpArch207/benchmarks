#ifndef __HASHTABLE_CHN__
#define __HASHTABLE_CHN__

#include <string>

using namespace std;

//entry size
enum { entry_sz = 16, log_sz = 1000 };

class LinkedHashEntry {
	private:
		//int entry_sz;
		int key_m[entry_sz];
		int val_m[entry_sz];
		LinkedHashEntry * next_m;
	public:
		//LinkedHashEntry();
		LinkedHashEntry(int key, int value);
		//LinkedHashEntry(const LinkedHashEntry & he);
		//LinkedHashEntry & operator=(const LinkedHashEntry & he);
		int getKey() const;
		int getVal() const;
		void setVal(int value);
		LinkedHashEntry *getNext() const;
		void setNext(LinkedHashEntry *next);
		friend ostream & operator<<(ostream & os, const LinkedHashEntry & entry);
};

class HashMap {
	private:
		LinkedHashEntry **table_m;
		//LinkedHashEntry *log_m;
		int * log_m;
		int table_size_m;
		int entry_size_m;
		int loops_m;
		int entries;
		int max_entries;
		int max_log;
		int cur_log;
	public:
		HashMap(int tab_sz, int loops);
		~HashMap();
		int get(int key);
		void put(int key, int value);
		void remove(int key);
		void p_pt_bp(int key, int value);
		void p_rm_bp(int key);
		void p_pt_cc(int key, int value);
		void p_rm_cc(int key);
		int getEntries();
		void show() const;
		inline void clflush(void * p) {
			asm volatile("sfence;"
					"clflush (%0)" :: "r"(p));}
		int gen_num();
		bool is_delete();
		bool is_full() { return entries >= table_size_m; }
		void persistent_cache(const string prog);
		void persistent_bypass(const string prog);
		void non_persistent(const string prog);
		bool needflush() { return cur_log >= max_log; }
		void flushLog();
		void update_max_ent() //update max entries
		{ max_entries = max_entries > entries ? max_entries : entries; }
		void bypassLog(int key, int val);
		void cacheLog(int key, int val);
		void rcn_bypass(const size_t & n, const string prog);
		void rcn_cache(const size_t & n, const string prog);
};

////manipulate the hashtable
//int gen_num();
//bool is_delete();
////void fill_ht();
//void non_persistent(HashMap & hm, const int loops, const string prog);
//void persistent(HashMap & hm, const int loops, const string prog);

////helper functs
//void usage();
//void get_arg(int, char **, int *, int *, int *);

#endif
