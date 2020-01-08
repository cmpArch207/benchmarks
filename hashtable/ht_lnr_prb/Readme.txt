This is the new version of micro benchmark hashtable-str. The size of each
entry can be customized according to the requirements. 

It can also perform non-persistent or persistent store.
(Choose either ht_str_np.cpp or ht_str_p.cpp in Makefile.)

sync.h & sync.cpp are used to synchornize multiple programs. When running
multiple programs, whoever terminates first, will generate a blank file
called prog.sync. When others detects this file, they will stop running
immediately. 

Don't forget to remove the prog.sync file before running program.

Usage: hashtable_str [-s size] [-l loops] [-e element size]. 

