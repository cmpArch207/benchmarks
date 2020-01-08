#include "sync.h"

fstream file;
string filename = "prog.sync";


bool is_terminated() { // check if one of the programs terminates
	//read the existing files
	file.open(filename.c_str(), ios::in);
	if (file.is_open()) //if there is a program end first
		return true;
	return false;
}

void term_prog(int loop, const string prog) {
	//terminate the current program
	cout << prog << ": loop = " << loop << endl;

	file.open(filename.c_str(), ios::out);

	exit(0);
}
	
void listen(int loop, const string prog) {
	if (is_terminated()) 
		term_prog(loop, prog);
}



