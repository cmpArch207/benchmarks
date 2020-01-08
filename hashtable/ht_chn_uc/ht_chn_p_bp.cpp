#include <iostream>
#include "hashtable_chn.h"
#include "sync.h"
#include "helper.h"
#include "defines.h"

using namespace std;

const string program = "htp_bp";//program's name, used by term_prog()


int main(int argc, char ** argv) {

        //<initialization>
        //mcsim_skip_instrs_begin();

        //string prog = program;
	int ht_sz, elt_sz, loops;

        get_arg(argc, argv, &ht_sz, &loops, &elt_sz);

        HashMap hm(ht_sz, loops);

	//inform another benchmark to start running
	get_started();

        //mcsim_skip_instrs_end();
        //<\initialization>

        
	//<count cycles>
	uint64_t start = rdtsc();
	hm.rcn_bypass(100, program);
	//<\count cycles>
	uint64_t end = rdtsc();
	uint64_t total_cycles = end - start;
	//cout << program;
	//cout << ": total cycles = " << total_cycles << endl;
	
	hm.show();

	double thp = (double)loops / (double)total_cycles;

	cout << program;
	cout << " : throughput = " << thp << endl;

        //terminate program
        term_prog(loops, program);

	
        return 0;
}
