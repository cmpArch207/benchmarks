#include <iostream>
#include "bigInt.h"

using namespace std;

int main() {
  uint64_t a = 111700;
  //int b = 68;
  uint64_t b = 674562;

  bigint A(a);
  bigint B(b);

  bigint S = 0;

  S = A + B + S;
  S = A + B + S;

  cout << "res = " << S << endl;


  return 0;
}
