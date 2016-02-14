# g++ -Wno-deprecated -Wall -lrt -lm -lpthread -O2  btree.cc driver.cc -o btree

CXX = g++
GCC_FLAGS = -Wno-deprecated -Wall -lrt -lm -lpthread
DEBUG_FLAGS = -DBTREE_DEBUG

OPT_FLAGS = -O2

btree: 
	$(CXX) $(GCC_FLAGS) $(OPT_FLAGS) $(DEBUG_FLAGS) btree.cc driver.cc mutex_func.cc -o btree

clean:
	rm -f *.o btree	*.debug
