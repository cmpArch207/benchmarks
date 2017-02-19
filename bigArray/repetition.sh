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
CMD0="./rcn_frdly_bp -s 1048576 -l 2097152 -e 400"
CMD1="./rcn_frdly_cc -s 1048576 -l 2097152 -e 400"
#CMD0="./rcn_frdly_bp -s 1048576 -l 8388608 -e 400"
#CMD1="./rcn_frdly_cc -s 1048576 -l 8388608 -e 400"
TIMES=1
OUTPUT0=stat.txt

#$CMD0  

#================================
rm $OUTPUT0

for  ((i = 0; i < $TIMES; i++))
do
  #rm *.sync
  #$CMD0 >> $OUTPUT0
  #rm *.sync
  $CMD1 >> $OUTPUT0

done


#./mean $OUTPUT0
#================================

#rm $OUTPUT0 $OUTPUT1 $OUTPUT2
