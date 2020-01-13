#/bin/bash - 
#=============================================================================
# 
# run microbenchmark
# 
#=============================================================================

#====================================
#run single benchmark
#====================================


perf stat -e LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses ./testBp
perf stat -e LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses ./testCc

#./mcsim -mdfile ../Apps/md/$MD.py -runfile ../Apps/list/spht/$BENCHMARK >& $OUTPUT

#./mcsim -mdfile ../Apps/md/$MD.py -runfile ../Apps/list/sht/smrt_spht/$BENCHMARK >& $OUTPUT

#./mcsim -mdfile ../Apps/md/$MD.py -runfile ../Apps/list/cln_cache/rcncy/smrt_spht/$BENCHMARK >& $OUTPUT



