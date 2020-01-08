#include <map>
#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>

using namespace std;

int main()
{
  map <int, int> rt;
  
  for (int i = 0; i != 10; ++i) {
    rt[i] = i;
  }

  const map<int, int>* rt_pointer = &rt;
  
  ofstream ofs("test.map", ios_base::binary);
  
  boost::archive::binary_oarchive archive(ofs);
  archive << (*rt_pointer);
  ofs.close();

  map<int, int> rt2;
  ifstream ifs("test.map", ios_base::binary);
  if(ifs.fail())
    assert(false);
  boost::archive::binary_iarchive inarch(ifs);
  inarch >> rt2;

  for(std::map<int,int>::const_iterator it=rt2.begin();
      it!= rt2.end(); ++it) {
    std::cerr<<"rt["<<it->first<<"]="<<it->second<<"\n";
  }
  
  return 0;
}
