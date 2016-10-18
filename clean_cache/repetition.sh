#!/bin/bash - 
#=============================================================================
# 
#repeatedly run the command
# 
#=============================================================================

#LOG_SZ=2097152
#LOG_SZ=262144
#LOG_SZ=32768
#LOG_SZ=4096
CMD0="./rcn_frdly_bp -s 2097152 -l 4194304 -e 1"
CMD1="./rcn_frdly_cc -s 2097152 -l 4194304 -e 1"
TIMES=20
OUTPUT0=stat.txt
OUTPUT1=perf_stat1.txt
OUTPUT2=perf_stat2.txt

rm $OUTPUT0

for  ((i = 0; i < $TIMES; i++))
do
  rm *.sync
  $CMD0 >> $OUTPUT0
  rm *.sync
  $CMD1 >> $OUTPUT0
  #./ht_chn_p -s 262144 -l 524288 -e 1 >> $OUTPUT0

done


#./calculator $OUTPUT0 > $OUTPUT2

#./calculator $OUTPUT1 >> $OUTPUT2

./mean $OUTPUT0

#rm $OUTPUT0 $OUTPUT1 $OUTPUT2
