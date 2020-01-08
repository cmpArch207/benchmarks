#include <iostream>
#include "sync.h"
#include "defines.h"
#include "clean_cache.h"
#include "helper.h"

using namespace std;

const string program = "rcn_bp"; //program's name, used by term_prog()

//static int pos = 8192; // num = the # of different positions

int main (int argc, char ** argv) {
	extern int * array;
	extern int * log;
	extern int log_size;
	extern int elt_size;
	extern int loops;
	extern int pos;
	extern string prog;


	//==========================================
	//<initializatio> 
	//==========================================
	//mcsim_skip_instrs_begin();

	prog = program;
	//get two members
	get_arg(argc, argv, &log_size, &loops, &elt_size);
	
	//build array
	build_array();

	//mcsim_skip_instrs_end();
	//==========================================
	//</initialization>
	//==========================================
	

	//<count cycles>
	uint64_t start = rdtsc();
	rcn_frdly_bp(pos); 
	//<\count cycles>
	uint64_t end = rdtsc();
	uint64_t total_cycles = end - start;

	double thp = (double)loops / (double)total_cycles;

	cout << program;
	cout << " : throughput = " << thp << endl;


	term_prog(loops, prog);

	return 0;
}
