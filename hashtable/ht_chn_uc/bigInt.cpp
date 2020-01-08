#include "bigInt.h"

using namespace std;

ostream & operator<< (ostream & out, const bigint & num) {
        if (num.big_value.size() != 0) {
                //print the sign
                if (num.negative) out << "-";

                //convert the vector into string
                string long_num;
                auto rit = num.big_value.crbegin();
                for (; rit != num.big_value.crend(); ++rit)
                        long_num.push_back('0' + *rit);

                out << long_num;
        }
        else
                out << "0";

        return out;
}

bigint::bigint() {
        negative = false;
}

bigint::bigint(const uint64_t &  num) {
        uint64_t n = num;
        negative = false;
        if (n == 0) return;

        int digit;
        do {
                digit = n % 10;
                big_value.push_back(digit);
                n /= 10;
        } while (n != 0);
}

//bigint::bigint(const int & num) {
        //int n = num;
        //negative = (n < 0) ? true : false;
        //if (n == 0) return;

        //int digit;
        //do {
                //digit = n % 10;
                //big_value.push_back(n);
                //n /= 10;
        //} while (n != 0);
//}

bigint operator+ (const bigint & a, const bigint & b) {
        bigint res;
        int na, nb, sum, carry = 0;

        auto ait = a.big_value.cbegin();
        auto bit = b.big_value.cbegin();
        while (ait != a.big_value.cend() || bit != b.big_value.cend()) {
                //get digits
                if (ait == a.big_value.cend()) {
                        na = 0;
                        nb = *bit;
                        ++bit;
                }
                else if (bit == b.big_value.cend()) {
                        nb = 0;
                        na = *ait;
                        ++ait;
                }
                else {
                        na = *ait;
                        nb = *bit;
                        ++ait;
                        ++bit;
                }

                //calculate
                sum = na + nb + carry;
                carry = sum / 10;
                sum %= 10;
                res.big_value.push_back(sum);
        }
        if (carry) res.big_value.push_back(carry);

        return res;
}

