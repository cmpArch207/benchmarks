#!/bin/bash - 
#=============================================================================
# 
#repeatedly run the command
# 
#=============================================================================

CMD="./small_log -s 262144 -l 600300 -e 1"
TIMES=100
OUTPUT0=perf_stat0.txt
OUTPUT1=perf_stat1.txt
OUTPUT2=perf_stat2.txt


for  ((i = 0; i < $TIMES; i++))
do
  rm *.sync
  $CMD >> $OUTPUT0
  #./ht_chn_p -s 262144 -l 524288 -e 1 >> $OUTPUT0

done


#calculation

./mean $OUTPUT0

#./calculator $OUTPUT0 > $OUTPUT2

#./calculator $OUTPUT1 >> $OUTPUT2


#rm $OUTPUT0 $OUTPUT1 $OUTPUT2
