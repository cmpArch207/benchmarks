#include <iostream>
#include "hashtable_chn.h"
#include "sync.h"
#include "helper.h"
#include "defines.h"

using namespace std;

const string program = "htp";//program's name, used by term_prog()


int main(int argc, char ** argv) {
        //<initialization>
        mcsim_skip_instrs_begin();

        //string prog = program;
	int ht_sz, elt_sz, loops;

        get_arg(argc, argv, &ht_sz, &loops, &elt_sz);

        HashMap hm(ht_sz, loops, elt_sz);

        mcsim_skip_instrs_end();
        //<\initialization>

        
	hm.non_persistent(program);
	hm.show();

        //terminate program
        term_prog(loops, program);

        return 0;
}
