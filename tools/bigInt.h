#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <iostream>
#include <vector>

using namespace std;

using bigvalue_t = vector<int>;

class bigint {
	friend ostream & operator<< (ostream &, const bigint&);
	private:
	bool negative;
	bigvalue_t big_value;
	public:
	bigint();
	bigint(const uint64_t &);
	//bigint(const int &);

	friend bigint operator+ (const bigint&, const bigint&);
	//friend bigint operator- (const bigint&, const bigint&);
};

#endif
