#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include "defines.h"

using namespace std;

#define VALUE_SIZE 2048
#define BASE 1000000000
#define DIFF 3

int main(int argc, char **argv)
{

  int i, num;
  int item_count = ITEM_COUNT, value_size = VALUE_SIZE;
  int base = BASE, diff = DIFF;

  for (i = 1; i != argc; ++i) {
    if (strncmp(argv[i], "--count", 7) == 0) {
      item_count = atoi(argv[i+1]);
      ++i;    
    } else {
      printf("Invalid parameters: '%s'\n", argv[i]);
      return -1;
    }
  } 

  ofstream ofs;
  ofs.open("sdg.dat");

  for (i = 0; i != item_count; ++i) {   
    ofs << dec << i + base << ";"
	<< dec << i + base << "_with_"
	<< dec << i + base/2 << ";"
	<< dec << i + base/2 << "\n"; 
    
    ofs << dec << i + base << ";"
	<< dec << i + base << "_with_"
	<< dec << i + diff + base/2 << ";"
	<< dec << i + diff + base/2 << "\n";

    /*ofs << hex << i + base << ";"
	<< hex << i + base << "_with_"
	<< dec << i + diff*2-1 + base/2 << ";"
	<< dec << i + diff*2-1 + base/2 << "\n";

    ofs << hex << i + base << ";"
	<< hex << i + base << "_with_"
	<< dec << i + diff*3-1 + base/2 << ";"
	<< dec << i + diff*3-1 + base/2 << "\n";*/
    
  }  
  ofs.close();
}
