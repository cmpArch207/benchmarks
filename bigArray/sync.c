#include "sync.h"

FILE * ifp, * ofp;
char * sync = "prog.sync";


int is_terminated() { // check if one of the programs terminates
	//read the existing files
	ifp = fopen(sync, "r");
	if (ifp) //if there is a program complete first
		return 1;
	return 0;
}

void term_prog(int loop, char * prog) {
	//terminate the current program
	/*cout << prog << ": loop = " << loop << endl;*/
	printf("%s : loop = %d\n", prog, loop);

	/*file.open(filename.c_str(), ios::out);*/
	ofp = fopen(sync, "w");

	if (ofp == NULL) {
		fprintf(stderr, "can't open file\n");
		exit(1);
	}

	exit(0);
}
	
void listen(int loop, char * prog) {
	if (is_terminated()) 
		term_prog(loop, prog);
}



