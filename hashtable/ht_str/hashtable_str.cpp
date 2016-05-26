#include <iostream>
#include <fstream>

#include "defines.h"
#include "sync.h"
#include "hashtable_str.h"

using namespace std;

//ctor
HT::HT(const size_t & ht_size, const size_t & string_len, 
        const size_t & num_loop, string program) {
        //get time seed
        srand(time(NULL));

        //set members
        str_len = string_len;
        loops   = num_loop;
        prog    = program;

        //build hashtable
        while (ht.size() < ht_size) {
                Str_Pair entry = gen_entry();
                ht.insert(entry);
        }
}

Str_Pair HT::gen_entry() {
        string key = gen_str();
        string val = gen_str();

        return Str_Pair(key, val);
}

string HT::gen_str() {
    // chars to compose a string
    char char_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    //generate a random string 
    char c;
    string str;
    for (int i = 0; i < str_len; ++i) {
      c = char_set[rand() % sizeof(char_set)];
      str.append(1, c);
    }

    return str;
}


void HT::show() const {
       //print out the entire hashtable
        cout << "key \t: value\n";
        for (auto iter = ht.begin(); iter != ht.end(); ++iter) { 
                cout << iter->first << "\t:" << iter->second;
                cout << endl;
        }
}


bool HT::is_delete() {
    //randomly choose to delete an entry or not
    int lottery = rand() % 100 + 1;
    
    return lottery > 50;
}


void HT::warm_up() {
    //run the program once before the experiment
    //to eliminate some cold misses
    mcsim_skip_instrs_begin();

    for (int i = 0; i < loops; i++) {
        if (is_delete() && ht.size() > 0) {
            //if need to delete, delete the 1st entry
            ht.erase(ht.begin());
        }
        else {
            //otherwise, randomly generate a key,
            string srch_key = gen_str();

            auto iter = ht.find(srch_key);
            if (iter != ht.end()) {
                //if the key exists, delete the
                //corresponding entry from hashtable
                ht.erase(iter);
            }
            else {
                //if not, insert a new entry
                string srch_val = gen_str();
                ht.insert(Str_Pair(srch_key, srch_val));
            }
        }
    }
    mcsim_skip_instrs_end();
}


void HT::non_persistent() {
    //non-persistent version
    warm_up();
}


void HT::persistent() {
    //count the operations
    int dels = 0;
    int inserts = 0;

    //persistent version
    for (int i = 0; i < loops; i++) {
        //check if another program end first
        mcsim_skip_instrs_begin();
        listen(i, prog);
        mcsim_skip_instrs_end();

        // loops
        if (is_delete() && ht.size() > 0) {
            //if need to delete, delete the 1st entry
            
            //get the target pair and store it into log
            Str_Pair del_pair = 
                Str_Pair(ht.begin()->first, ht.begin()->second);

            //persistently delete an entry
            mcsim_tx_begin();
            mcsim_log_begin();
            log.push_back(del_pair);
            mcsim_mem_fence();
            mcsim_log_end();
            mcsim_mem_fence();
            ht.erase(ht.begin());
            mcsim_tx_end();

            ++dels;
        }
        else {
            //otherwise, randomly generate a key,
            string srch_key = gen_str();

            auto iter = ht.find(srch_key);
            if (iter != ht.end()) {
                //if the key exists, delete the
                //corresponding entry from hashtable
                Str_Pair del_pair = 
                    Str_Pair(srch_key, iter->second);

                mcsim_tx_begin();
                mcsim_log_begin();
                log.push_back(del_pair);
                mcsim_mem_fence();
                mcsim_log_end();
                mcsim_mem_fence();
                ht.erase(iter);
                mcsim_tx_end();

                ++dels;
            }
            else {
                //if not, insert a new entry
                string srch_val = gen_str();
                Str_Pair ins_pair = Str_Pair(srch_key, srch_val);

                //persistently insert an entry
                mcsim_tx_begin();
                mcsim_log_begin();
                log.push_back(ins_pair);
                mcsim_mem_fence();
                mcsim_log_end();
                mcsim_mem_fence();
                ht.insert(ins_pair);
                mcsim_tx_end();
                mcsim_clwb(&(ht[srch_key]));

                ++inserts;
            }
        }
    }
    cout << "dels = " << dels;
    cout << ", inserts = " << inserts << endl;
}


// helper functions
void usage() {
        cout << "Usage: hashtable_str [-s size] [-l loops].\n";
        exit(EXIT_FAILURE);
}

void get_arg(int argc, char ** argv, int * size, int * loops) {
        if (argc != 5) {
                usage();
        }

        // get options
        int opt;
        while ((opt = getopt(argc, argv, "s:l:")) != -1) {
                switch (opt) {
                        case 'l':
                                *loops = atoi(optarg);
                                break;
                        case 's':
                                *size = atoi(optarg);
                                break;
                        default:
                                usage();
                }
        }
}
