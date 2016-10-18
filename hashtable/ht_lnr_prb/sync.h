#ifndef __SYNC_H__
#define __SYNC_H__

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

bool is_terminated();
void term_prog(int loop, const string prog);
void listen(int loop, const string prog);

////int f_ind = 0;
////fstream file[PROG_N];
//fstream file;
//string filename = "prog.sync";


//bool is_terminated() { // check if one of the programs terminates
	////read the existing files
	//file.open(filename.c_str(), ios::in);
	//if (file.is_open()) //if there is a program end first
		//return true;
	//return false;

	////for (size_t i = 0; i < PROG_N; ++i) {
		////string filename = get_filename(i);
		////file[i].open(filename.c_str(), ios::in);

		////if (file[i].is_open()) //if there is a program end first
			////return true;
	////}
	////return false;
//}

//void term_prog() {
	////terminate the current program
	////string filename = get_filename(f_ind);
	//file.open(filename.c_str(), ios::out);
	////++f_ind;
	//exit(0);
//}
	
//void listen() {
	//if (is_terminated()) 
		//term_prog();
//}


#endif

