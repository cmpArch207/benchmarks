#include <iostream>
#include <vector>
#include <sys/time.h>
#include <time.h>
#include <unistd.h> // getopt()

#include "defines.h"

using namespace std;

class LongVec {
	private:
		vector<int> vec;
	public:
		LongVec(const int &);
		void streaming(const int &);
		void rand_acc(const int &);  // random access
		void show() const;

};

LongVec::LongVec(const int & size) {
	for (int i = 0; i < size; ++i) {
		int elt = rand();
		vec.push_back(elt);
	}
}

//void LongVec::increase() {
	//for (size_t i = 0; i < vec.size(); ++i) 
		//++vec.at(i);
//}

void LongVec::streaming(const int & loops) {
	size_t i, j;
	for (i = 0, j = 0; i < loops; ++i, ++j) {
		if (j >= vec.size()) j = 0;
		++vec.at(j);
	}
}

void LongVec::show() const {
	for (size_t i = 0; i < vec.size(); ++i) 
		cout << vec.at(i) << " ";
	cout << endl;
}

void LongVec::rand_acc(const int & loops) {
	for (size_t i = 0; i < loops ; ++i) {
		size_t pos = rand() % vec.size();
		//cout << "pos = " << pos << endl;
		++vec.at(pos);
	}
}

int main (int argc, char ** argv) {
	if (argc != 5) {
		cout << "Usage: clean_cache [-s size] [-l loops].\n";
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));
	int size, loops;
	
	//get options
	int opt;
	while ((opt = getopt(argc, argv, "ls")) != -1) {
		switch (opt) {
			case 'l':
				loops = atoi(argv[optind]);
				break;
			case 's':
				size = atoi(argv[optind]);
				break;
			
			default:  //case '?':
				cout << "Usage: clean_cache [-s size] [-l loops].\n";
				exit(EXIT_FAILURE);
		}
	}
	
	//cout << "loops = " << loops;
	//cout << " size = " << size << endl;

	//skip the initialization stage
	mcsim_skip_instrs_begin();

	LongVec V(size);

	mcsim_skip_instrs_end();
	//////////
	
	//V.show();

	V.streaming(loops);
	//V.rand_acc(loops);

	//V.show();

	return 0;
}
