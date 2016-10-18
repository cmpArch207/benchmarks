#include <iostream>
#include <climits>
//#include <unistd.h>
#include <string>
#include <emmintrin.h>
//#include <intrin.h>

#include "defines.h"
#include "sync.h"
#include "hashtable_chn.h"
#include "bigInt.h"
#include "helper.h"

using namespace std;

//int debug = 0;
bigint tot_time = 0;
uint64_t bf_fl_time = 0;
uint64_t af_fl_time = 0;


//LinkedHashEntry::LinkedHashEntry(int key = 1, int val = 1)
	//:key_m(key), 
	//val_m(val),
	//next_m(nullptr)
//{}

//LinkedHashEntry::LinkedHashEntry() {
    //cout << "default ctor\n";

    //for (int i = 0; i < entry_sz; ++i) {
	//key_m[i] = 0;
	//val_m[i] = 0;
    //}

    //next_m = nullptr;
//}

LinkedHashEntry::LinkedHashEntry(int key = 1, int val = 1) {
    //cout << "default ctor\n";
    //key_m = new int[entry_sz];
    //val_m = new int[entry_sz];

    for (int i = 0; i < entry_sz; ++i) {
	key_m[i] = key;
	val_m[i] = val;
    }

    next_m = nullptr;
}

//LinkedHashEntry::LinkedHashEntry(const LinkedHashEntry & he) {
    //cout << "cp ctor\n";
    ////key_m = new int[he.entry_sz];
    ////val_m = new int[he.entry_sz];

    //for (int i = 0; i < entry_sz; ++i) {
	//key_m[i] = he.key_m[0];
	//val_m[i] = he.val_m[0];
    //}
    ////key_m = he.key_m;
    ////val_m = he.val_m;
    //next_m = nullptr;//assign none ptr to replicate entry
//}

//LinkedHashEntry & LinkedHashEntry::operator=(const LinkedHashEntry & he) {
    //cout << "operator=\n";
    //if (this == &he)
	//return *this;
    ////key_m = new int[he.entry_sz];
    ////val_m = new int[he.entry_sz];
    //for (int i = 0; i < entry_sz; ++i) {
	//key_m[i] = he.key_m[0];
	//val_m[i] = he.val_m[0];
    //}
    ////key_m = he.key_m;
    ////val_m = he.val_m;
    //next_m = nullptr;//assign none ptr to replicate entry

//}


int LinkedHashEntry::getKey() const {
    return key_m[0];
}

int LinkedHashEntry::getVal() const {
    return val_m[0];
}

void LinkedHashEntry::setVal(int value) {
    for (int i = 0; i < entry_sz; ++i)
	val_m[i] = value;
}


LinkedHashEntry * LinkedHashEntry::getNext() const {
    return next_m;
}

void LinkedHashEntry::setNext(LinkedHashEntry * next) {
    next_m = next;
}

ostream & operator<<(ostream & os, const LinkedHashEntry & entry) {
    os << "key = " << entry.key_m[0];
    os <<", val = " << entry.val_m[0];

    return os;
}

HashMap::HashMap(int table_size = 100, int loops = 100) 
	:table_size_m(table_size),
	loops_m(loops),
	//entry_size_m(entry_size),
	entries(0),
	max_entries(0),
	max_log(log_sz), //max # of log entries
	cur_log(0) //current # of log entries
{
	table_m = new LinkedHashEntry*[table_size_m];
	for (int i = 0; i < table_size_m; ++i)
		table_m[i] = nullptr;
	//log_m = new LinkedHashEntry[max_log];
	log_m = new int[max_log * 2];
	//generate time seed
	srand(time(NULL));

}

int HashMap::get(int key) {
	int hash = (key % table_size_m);

	if (table_m[hash] == nullptr)
		return -1;
	else {
		LinkedHashEntry *entry = table_m[hash];
		while (entry != nullptr && entry->getKey() != key)
			entry = entry->getNext();

		if (entry == nullptr)
			return -1;
		else
			return entry->getVal();
	}
}


void HashMap::put(int key, int value) {
    int hash = (key % table_size_m); 
    LinkedHashEntry *prevEntry = nullptr;
    LinkedHashEntry *entry = table_m[hash];

    //int j = 0;
    while (entry != nullptr && entry->getKey() != key) {
	//cout << "j = " << j++ << endl;
	prevEntry = entry;
	entry = entry->getNext();
    }

    if (entry == nullptr) {//key not found
	//cout << "not chain\n";
	++entries;
	if (prevEntry == nullptr) {
	    //insert the 1st entry
	    table_m[hash] = new LinkedHashEntry(key, value);
	} else {//append new entry
	    prevEntry->setNext(new LinkedHashEntry(key, value));
	}
    } else if (entry->getKey() == key) {//key exists
	//cout << "chain\n";
	entry->setVal(value);//overwrite the value
    }
    update_max_ent();
}


void HashMap::remove(int key) {
	int hash = (key % table_size_m);
	if (table_m[hash] != nullptr) {
		LinkedHashEntry *prevEntry = nullptr;
		LinkedHashEntry *entry = table_m[hash];

		while (entry->getNext() != nullptr && entry->getKey() != key) {
			prevEntry = entry;
			entry = entry->getNext();
		}

		if (entry->getKey() == key) {//the key is found
			--entries;
			if (prevEntry == nullptr) {//it's the 1st entry
				LinkedHashEntry *nextEntry = entry->getNext();
				delete entry;
				table_m[hash] = nextEntry;
			} else {
				LinkedHashEntry *next = entry->getNext();
				delete entry;
				prevEntry->setNext(next);
			}
		}
	}
}


//bypass the cache 
void HashMap::p_pt_bp(int key, int value) {
	//persistent version put funct
	int hash = (key % table_size_m); 
	LinkedHashEntry *prevEntry = nullptr;
	LinkedHashEntry *entry = table_m[hash];

	while (entry != nullptr && entry->getKey() != key) {
		prevEntry = entry;
		entry = entry->getNext();
    }

    if (entry == nullptr) {//key not found
		++entries;
		if (prevEntry == nullptr) {
			//insert the 1st entry
			table_m[hash] = new LinkedHashEntry(key, value);
			//update log
			//log_m[cur_log++] = *table_m[hash];
		} else {//append new entry
			prevEntry->setNext(new LinkedHashEntry(key, value));
			//update log
			//log_m[cur_log++] = *(prevEntry->getNext());
		}
    } else if (entry->getKey() == key) {//key exists
		entry->setVal(value);//overwrite the value
		//update log
		//log_m[cur_log++] = *entry;
    }
	bypassLog(key, value);
    update_max_ent();
}


void HashMap::p_rm_bp(int key) {
	int hash = (key % table_size_m);
	//cout << "rm: key = " << key << endl;
	if (table_m[hash] != nullptr) {
		LinkedHashEntry *prevEntry = nullptr;
		LinkedHashEntry *entry = table_m[hash];

		while (entry->getNext() != nullptr && entry->getKey() != key) {
			prevEntry = entry;
			entry = entry->getNext();
		}

		//if (entry->getKey() == key) {//the key is found
		//if (entry->getKey() != key) 
		    //cout << "randomly delete 1\n";

		//update log
		bypassLog(entry->getKey(), entry->getVal());
		if (prevEntry == nullptr) {//it's the 1st entry
		    LinkedHashEntry *nextEntry = entry->getNext();
		    delete entry;
		    table_m[hash] = nextEntry;
		} else {
		    LinkedHashEntry *next = entry->getNext();
		    delete entry;
		    prevEntry->setNext(next);
		}
		//update entries
		--entries;
		//}
		//else { // delete the last entry
		    //cout << "randomly delete 1\n";
		    //if (prevEntry) 
			//prevEntry->setNext(nullptr);
		    //delete entry;
		//}
		////update entries
		//--entries;
		////don't make entry a dangling ptr
		//entry = nullptr;
	}
}


//cache the log
void HashMap::p_pt_cc(int key, int value) {
	//persistent version put funct
	int hash = (key % table_size_m); 
	LinkedHashEntry *prevEntry = nullptr;
	LinkedHashEntry *entry = table_m[hash];

	while (entry != nullptr && entry->getKey() != key) {
		prevEntry = entry;
		entry = entry->getNext();
    }

    if (entry == nullptr) {//key not found
		++entries;
		if (prevEntry == nullptr) {
			//insert the 1st entry
			table_m[hash] = new LinkedHashEntry(key, value);
			//update log
			//log_m[cur_log++] = *table_m[hash];
		} else {//append new entry
			prevEntry->setNext(new LinkedHashEntry(key, value));
			//update log
			//log_m[cur_log++] = *(prevEntry->getNext());
		}
    } else if (entry->getKey() == key) {//key exists
		entry->setVal(value);//overwrite the value
		//update log
		//log_m[cur_log++] = *entry;
    }
	cacheLog(key, value);
    update_max_ent();
}


void HashMap::p_rm_cc(int key) {
	int hash = (key % table_size_m);
	//cout << "rm: key = " << key << endl;
	if (table_m[hash] != nullptr) {
		LinkedHashEntry *prevEntry = nullptr;
		LinkedHashEntry *entry = table_m[hash];

		while (entry->getNext() != nullptr && entry->getKey() != key) {
			prevEntry = entry;
			entry = entry->getNext();
		}

		//if (entry->getKey() == key) {//the key is found
		//if (entry->getKey() != key) 
		    //cout << "randomly delete 1\n";

		//update log
		cacheLog(entry->getKey(), entry->getVal());
		if (prevEntry == nullptr) {//it's the 1st entry
		    LinkedHashEntry *nextEntry = entry->getNext();
		    delete entry;
		    table_m[hash] = nextEntry;
		} else {
		    LinkedHashEntry *next = entry->getNext();
		    delete entry;
		    prevEntry->setNext(next);
		}
		//update entries
		--entries;
		//}
		//else { // delete the last entry
		    //cout << "randomly delete 1\n";
		    //if (prevEntry) 
			//prevEntry->setNext(nullptr);
		    //delete entry;
		//}
		////update entries
		//--entries;
		////don't make entry a dangling ptr
		//entry = nullptr;
	}
}


int HashMap::getEntries() {
	return entries;
}


HashMap::~HashMap() {
	for (int i = 0; i < table_size_m; i++)
		if (table_m[i] != nullptr) {
			LinkedHashEntry *prevEntry = nullptr;
			LinkedHashEntry *entry = table_m[i];
			while (entry != nullptr) {
				prevEntry = entry;
				entry = entry->getNext();
				delete prevEntry;
			}
		}
	delete[] table_m;

	delete[] log_m;
}


void HashMap::show() const {
	//for (int i = 0; i < table_size_m; i++) {
	    //cout << "hash = " << i << endl;
	    //if (table_m[i] != nullptr) {
		    //LinkedHashEntry *entry = table_m[i];
		    //while (entry != nullptr) {
			    //cout << *entry << endl;
			    //entry = entry->getNext();
		    //}
	    //}
	//}
	cout << "total entires = " << entries << endl;
	cout << "max   entries = " << max_entries << endl;
}

//generate a random num
int HashMap::gen_num() {
	return rand() % INT_MAX;
	//return rand() % 20;
}

bool HashMap::is_delete() {
    //randomly choose to delete an entry or not
    int lottery = rand() % 100 + 1;
    
    return lottery > 50;
}

void HashMap::non_persistent(const string prog) {
    for (int i = 0; i < loops_m; ++i) {
	//check if another program end first
	mcsim_skip_instrs_begin();
	listen(i, prog);
	mcsim_skip_instrs_end();

	//generate key value pair
	int key = gen_num();
	int val = gen_num();

	if ((is_delete() || is_full()) && getEntries()) {
	    remove(key);
	} else {
	    put(key, val);
	}
    }
    //hm.show();
}

void HashMap::persistent_bypass(const string prog) {
    //cout << "table size = " << table_size_m << endl;
	//<count the cycles>
	//uint64_t start = 0, end = 0;
	//bool is_flush = false;
	//bool proc_bf = false;
	//bool proc_af = false;
	//bool once = true;
	//<\count the cycles>
    for (int i = 0; i < loops_m; ++i) {
		//if (is_count_cycles == false) {
		//<count the cycles>
		//count the time for 20 loops before clflush
		//if (cur_log == max_log - 20 && proc_bf == false && once) {
			//proc_bf = true;
			//start = rdtsc();
		//}
		//if (cur_log == max_log - 1 && proc_bf && once) {
			//is_flush = true;
			//proc_bf = false;
			//end = rdtsc();
			//bf_fl_time = end - start;
			//cout << "i = " << i;
			//cout << ": time before clflush = " << bf_fl_time << endl;
		//}

		//count the time for 20 loops after clflush
		//if (cur_log == 1 && is_flush && proc_af == false && once) {
			//proc_af = true;
			//start = rdtsc();
		//}
		//if (cur_log == 20 && is_flush && once) {
			//proc_af = false;
			//end = rdtsc();
			//af_fl_time = end - start;
			//cout << "i = " << i;
			//cout << ": time after clflush = " << af_fl_time << endl;
			//once = false;
		//}
		//<\count the cycles>

		//cout << "entries = " << entries << ", max_entries = " << max_entries << endl;
		//if (is_full()) cout << "full\n";
		//check if another program end first
		//mcsim_skip_instrs_begin();
		listen(i, prog);
		//mcsim_skip_instrs_end();

		//generate key value pair
		int key = gen_num();
		int val = gen_num();

		if ((is_delete() || is_full()) && getEntries()) {
			//cout << "rm:======================\n";
			p_rm_bp(key);
		} else {
			//cout << "put:======================\n";
			p_pt_bp(key, val);
		}
		////flush log
		//if (needflush())
			//flushLog();
		////debug
		//show();
    }

	//cout << "total flush time = " << tot_time << endl;

}

void HashMap::persistent_cache(const string prog) {
    for (int i = 0; i < loops_m; ++i) {
		//check if another program end first
		//mcsim_skip_instrs_begin();
		listen(i, prog);
		//mcsim_skip_instrs_end();

		//generate key value pair
		int key = gen_num();
		int val = gen_num();

		if ((is_delete() || is_full()) && getEntries()) {
			//cout << "rm:======================\n";
			p_rm_cc(key);
		} else {
			//cout << "put:======================\n";
			p_pt_cc(key, val);
		}
    }
}

void HashMap::rcn_bypass(const size_t & n, const string prog) {
	//n is the # of different positions
	int * pos = new int [n];
	int key, val;
	bool fst_loop = true;
	//i is the ptr for loops,
	//j is the ptr for n different positions
	size_t i, j;

	for (i = 0, j = 0; i < loops_m ; ++i, ++j) {
		//listen(i, prog);
		//in the first loop, 
		//put the positions into the vector
		if (i == n) fst_loop = false;

		//repeatedly access n postions
		if (j == n) j = 0;

		if (fst_loop) {
			key = rand() % table_size_m;
			pos[j] = key;
		}
		else
			key = pos[j];

		val = gen_num();
		p_pt_bp(key, val);
	}

	delete [] pos;
}

void HashMap::rcn_cache(const size_t & n, const string prog) {
	//n is the # of different positions
	int * pos = new int [n];
	int key, val;
	bool fst_loop = true;
	//i is the ptr for loops,
	//j is the ptr for n different positions
	size_t i, j;

	for (i = 0, j = 0; i < loops_m ; ++i, ++j) {
		//listen(i, prog);
		//in the first loop, 
		//put the positions into the vector
		if (i == n) fst_loop = false;

		//repeatedly access n postions
		if (j == n) j = 0;

		if (fst_loop) {
			key = rand() % table_size_m;
			pos[j] = key;
		}
		else
			key = pos[j];

		val = gen_num();
		p_pt_cc(key, val);
	}

	delete [] pos;
}

void HashMap::flushLog() {
    //uint64_t start, end;
    //start = rdtsc();
    for (int i = 0; i < cur_log; ++i) {
	clflush((void*)&log_m[i]);
    }
    cur_log = 0;

    //end = rdtsc();
    //uint64_t time = end - start;

    //cout << "flush time = " << time << endl;

	//tot_time = bigint(time) + tot_time;
}

void HashMap::bypassLog(int key, int val) {
	if (needflush()) cur_log = 0;

	for (int i = 0; i < entry_sz; ++i) {
		_mm_stream_si32(&log_m[cur_log++], key);
		_mm_stream_si32(&log_m[cur_log++], val);
	}
}

void HashMap::cacheLog(int key, int val) {
	if (needflush()) cur_log = 0;

	for (int i = 0; i < entry_sz; ++i) {
		log_m[cur_log++] = key;
		log_m[cur_log++] = val;
	}
}

