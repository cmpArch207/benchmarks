#include <iostream>
#include "hashtable_str.h"
#include "sync.h"
#include "defines.h"

using namespace std;

const size_t string_len = 4;
const string program = "sht"; //program's name, used by term_prog()

//get two members
extern int ht_sz, loops, elt_sz;
extern string prog;
extern int * ht;


int main(int argc, char ** argv) {
        prog = program;
        elt_sz = 1;// default element size

        //<initialization>
        mcsim_skip_instrs_begin();

        get_arg(argc, argv, &ht_sz, &loops, &elt_sz);

        ht = build_ht();

        mcsim_skip_instrs_end();
        //<\initialization>

        
        //show();
        non_persistent();
        //show();

        //terminate program
        term_prog(loops, prog);

        return 0;
}
