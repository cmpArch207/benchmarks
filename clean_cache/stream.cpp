#include <iostream>
#include "sync.h"
#include "defines.h"
#include "clean_cache.h"

using namespace std;

const string program = "stm"; //program's name, used by term_prog()


int main (int argc, char ** argv) {
	extern int * array;
	extern int * target_array;
	extern int ary_size;
	extern int elt_size;
	extern int loops;
	extern string prog;

	//==========================================
	//<initializatio> 
	//==========================================
	//mcsim_skip_instrs_begin();

	prog = program;

	//get parameters
	get_arg(argc, argv, &ary_size, &loops, &elt_size);
	
	//build array
	build_array();
	

	//mcsim_skip_instrs_end();
	//==========================================
	//</initialization>
	//==========================================
	

	//V.show();

	streaming();
	//V.rand_acc();
	//V.thrashing();
	
	//int num = 50; // num = the # of different positions
	//V.rcn_frdly(num); 

	//V.show();

	//end_ = clock();
	//time_spent = (double)(end_ - begin_) / CLOCKS_PER_SEC;
	//time(&end_);
	//time_spent = (double)difftime(end_, begin_);
	//cout << "----------------------------------\n";
	//cout << "tot time = " << time_spent << endl;
	//cout << "----------------------------------\n";

	term_prog(loops, prog);

	return 0;
}
