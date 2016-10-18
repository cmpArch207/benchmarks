#include "sync.h"

fstream efile;
fstream sfile;
string endFile = "end.sync";
string startFile = "start.sync";

void get_started() {
	cout << "Initialization complete\n";
	sfile.open(startFile.c_str(), ios::out);
}

bool is_started() { // check if one of the programs terminates
	//read the existing files
	sfile.open(startFile.c_str(), ios::in);
	if (sfile.is_open()) //if there is a sync file
		return true;
	return false;
}

bool is_terminated() { // check if one of the programs terminates
	//read the existing files
	efile.open(endFile.c_str(), ios::in);
	if (efile.is_open()) //if there is a program end first
		return true;
	return false;
}

void term_prog(int loop, const string prog) {
	//terminate the current program
	cout << prog << ": loop = " << loop << endl;

	efile.open(endFile.c_str(), ios::out);

	exit(0);
}
	
void listen(int loop, const string prog) {
	if (is_terminated()) 
		term_prog(loop, prog);
}



