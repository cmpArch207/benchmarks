This is the new version of micro benchmark hashtable-str. The size of each
entry can be customized according to the requirements. 
(The size of entry need to be specified in np_ht.cpp file.)

It can also perform non-persistent or persistent store.
(Choose either np_ht.cpp or p_ht.cpp in Makefile.)

This program is highly encapsulated. Thus, it is more safe and easier to modify or add more features in the future.

sync.h & sync.cpp are used to synchornize multiple programs. When running
multiple programs, whoever terminates first, will generate a blank file
called prog.sync. When others detects this file, they will stop running
immediately. 

Don't forget to remove the prog.sync file before running program.

Usage: hashtable_str [-s size] [-l loops]. 

