#include <iostream>
#include "hashtable_str.h"
#include "defines.h"

using namespace std;

const size_t string_len = 4;
const string prog = "sht"; //program's name, used by term_prog()



int main(int argc, char ** argv) {
        //<initialization>
        mcsim_skip_instrs_begin();

        //get two members
        int ht_sz, loops;
        get_arg(argc, argv, &ht_sz, &loops);

        HT ht_str(ht_sz, string_len, loops, prog);

        mcsim_skip_instrs_end();
        //<\initialization>

        //ht_str.show();
        //ht_str.non_persistent();
        ht_str.persistent();
        //ht_str.show();

        return 0;
}
