#ifndef __CLEAN_CACHE_H__
#define __CLEAN_CACHE_H__

#include <unistd.h> // getopt

using namespace std;

//class LongVec {
	//private:
		//int * array;
		//string prog;
		//size_t loops;
		//int element_size;
		//int array_size;
	//public:
		//LongVec(const int &, const int &, string, const size_t &);
		//void rcn_frdly(const int &);
		//void streaming();
		//void rand_acc();  // random access
		//void thrashing();
		//void show() const;

//};

void build_array();
void streaming();
void rcn_frdly_cc(const size_t &);
void rcn_frdly_bp(const size_t &);
void show();
void bypassLog(int n);
void cacheLog(int n);



/*//helper functs*/
//void usage();
//void get_arg(int, char **, int *, int *, int *);

#endif
