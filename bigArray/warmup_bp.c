#include "stdio.h"

/*#include "sync.h"*/
/*#include "defines.h"*/
#include "big_array.h"
#include "helper.h"

char *program = "rcn_bp"; //program's name, used by term_prog()

//static int pos = 8192; // num = the # of different positions

int main (int argc, char ** argv) {
	extern int *array;
	extern int *data_log;
	extern int log_size;
	extern int elt_size;
	extern int loops;
	extern int pos;
	extern char *prog;
	extern uint64_t total_cycles;

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


	/*//<count cycles>*/
	/*uint64_t start = rdtsc();*/
	warmup_bp(pos);
	/*//<\count cycles>*/
	/*uint64_t end = rdtsc();*/
	/*uint64_t total_cycles = end - start;*/

	/*//don't count the warmup section*/
	/*double thp = (double)(loops - pos) / (double)total_cycles;*/

	/*printf("%s", program);*/
	/*printf(" : throughput = %f\n", thp);*/


	/*term_prog(loops, prog);*/

	return 0;
}
