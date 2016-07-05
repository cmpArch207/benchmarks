#include <iostream>
#include <fstream>
#include <climits>

#include "defines.h"
#include "sync.h"
#include "hashtable_str.h"


using namespace std;

//ht_sz    = # of elements
//elt_size = # of integers for each element
int ht_sz, loops, elt_sz;
//name of the benchmark, used by sync files
string prog;
//the address of the hashtable
int * ht;

//globle value to store the # of entries in hashtable
int entries = 0;

//global value to store the entry size
int entry_sz;

//build hashtable
int * build_ht() {
    //get entry_size
    //entry size = invalid value(int) + 
    //             key size (int)*element size +
    //             val (int)*element size
    entry_sz = 1 + elt_sz * 2;
    //get time seed
    srand(time(NULL));

    //allocate mem
    int tot_size = entry_sz * ht_sz;
    //align to next multiple of 16
    //(not necessary)
    tot_size = (tot_size + 15) & ~15;

    //allocate
    tot_size *= sizeof(int);
    ht = (int*)sbrk(tot_size);

    if (ht == (void *)-1) {
        cout << "Error: sbrk() failed\n";
        return NULL;
    }

    fill_ht();

    return ht;
}

//fill hashtable
void fill_ht() {
    while (entries < ht_sz) {
        //generate key & value
        int key = gen_num();
        int val = gen_num();

        //cout << "key = " << key;
        //cout << ", val = " << val << endl;

        //find an avail pos and insert
        int pos = search_ent(key);
        if (pos >= 0)
            insert_ent(pos, key, val);
    }
}

//search a key
int search_ent(int key) {
    //get hash value and corresponding pos
    int pos = (key % ht_sz) * entry_sz;
    //prevent infinite loop
    int lps = 0;
    //find pos
    while (ht[pos]) {
        //if key exists
        if (ht[pos+1] == key) {
            //cout << "key exists\n";
            return pos;
        }
        pos += entry_sz;
        ++lps;
        if (pos >= ht_sz*entry_sz) pos = 0;
        if (lps >= ht_sz) {
            //cout << "infinite loop\n";
            //if (entries == ht_sz)
                //cout << "hashtable is full\n";
            return -1;
        }
    }
    return pos;
}

//insert entry
void insert_ent(int pos, int key, int val) {
    if (pos < 0) return;

    //otherwise, there is an avail pos
    //set invalid value
    ht[pos] = 1;
    //insert key value pair
    for (int i = 0; i < elt_sz; ++i) {
        //insert key
        ht[pos+1+i] = key;
        //insert value
        ht[pos+1+elt_sz+i] = val;
    }
    //update # of entries
    ++entries;
}


//delete entry
void delete_ent(int pos) {
    //randomly delete an entry
    if (pos == -1)
        pos = (rand() % ht_sz) * entry_sz;

    if (ht[pos] == 0)
        return;
    
    //don't erase the current entry
    //set invalid value as 0
    ht[pos] = 0;
    --entries;
}

//generate a random num
int gen_num() {
    //return rand() % INT_MAX;
    return rand() % 30000;
}


void show() {
       cout << "# of entries = " << entries << endl;
       //print out the entire hashtable
       cout << "key \t: value\n";
       for (int i = 0; i < ht_sz; ++i) {
           if (ht[i*entry_sz]) {
               cout << ht[i*entry_sz+1] << "\t: ";
               cout << ht[i*entry_sz+1+elt_sz] << endl;
           }
           else
               cout << "0000\t: 0000\n";
       }
}


bool is_delete() {
    //randomly choose to delete an entry or not
    int lottery = rand() % 100 + 1;
    
    return lottery > 50;
}




void non_persistent() {
    //non-persistent version
    int i;
    for (i = 0; i < loops; i++) {
        //check if another program end first
        listen(i, prog);

        //generate key value pair
        int key = gen_num();
        int val = gen_num();

        int pos = search_ent(key);

        if (is_delete() && entries > 0) {
            //if need to delete, randomly delete an entry
            delete_ent(pos);
        }
        else {//insert a new entry or delete an entry
            if (pos == -1 and entries == ht_sz)//hashtable is full 
                delete_ent(pos);
            else  
                insert_ent(pos, key, val);
        }
    }
}


//void HT::persistent() {
    //cout << "(p) experiment begins\n";

    ////count the operations
    //int dels = 0;
    //int inserts = 0;

    ////persistent version
    //int i;
    //for (i = 0; i < loops; i++) {
        ////check if another program end first
        //mcsim_skip_instrs_begin();
        //listen(i, prog);
        //mcsim_skip_instrs_end();

        //// loops
        //if (is_delete() && ht.size() > 0) {
            ////if need to delete, delete the 1st entry
            
            ////get the target pair and store it into log
            //Str_Pair del_pair = 
                //Str_Pair(ht.begin()->first, ht.begin()->second);

            ////persistently delete an entry
            //mcsim_tx_begin();
            //mcsim_log_begin();
            //log.push_back(del_pair);
            //mcsim_mem_fence();
            //mcsim_log_end();
            //mcsim_mem_fence();
            //ht.erase(ht.begin());
            //mcsim_tx_end();

            //++dels;
        //}
        //else {
            ////otherwise, randomly generate a key,
            //string srch_key = gen_str();

            //auto iter = ht.find(srch_key);
            //if (iter != ht.end()) {
                ////if the key exists, delete the
                ////corresponding entry from hashtable
                //Str_Pair del_pair = 
                    //Str_Pair(srch_key, iter->second);

                //mcsim_tx_begin();
                //mcsim_log_begin();
                //log.push_back(del_pair);
                //mcsim_mem_fence();
                //mcsim_log_end();
                //mcsim_mem_fence();
                //ht.erase(iter);
                //mcsim_tx_end();

                //++dels;
            //}
            //else {
                ////if not, insert a new entry
                //string srch_val = gen_str();
                //Str_Pair ins_pair = Str_Pair(srch_key, srch_val);

                ////persistently insert an entry
                //mcsim_tx_begin();
                //mcsim_log_begin();
                //log.push_back(ins_pair);
                //mcsim_mem_fence();
                //mcsim_log_end();
                //mcsim_mem_fence();
                //ht.insert(ins_pair);
                //mcsim_tx_end();
                //mcsim_clwb(&(ht[srch_key]));

                //++inserts;
            //}
        //}
    //}
    //cout << "dels = " << dels;
    //cout << ", inserts = " << inserts << endl;

    //// terminate the program
    //term_prog(i, prog);
//}


// helper functions
void usage() {
        cout << "Usage: hashtable_str [-s size] [-l loops] [-e element size].\n";
        exit(EXIT_FAILURE);
}

void get_arg(int argc, char ** argv, int * size, int * loops, int * elt_size) {
        if (argc != 5 and argc != 7) 
                usage();

        // get options
        int opt;
        while ((opt = getopt(argc, argv, "s:l:e:")) != -1) {
                switch (opt) {
                        case 'l':
                                *loops = atoi(optarg);
                                break;
                        case 's':
                                *size = atoi(optarg);
                                break;
                        case 'e':
                                *elt_size = atoi(optarg);
                                break;
                        default:
                                usage();
                }
        }
}
