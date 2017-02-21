#!/bin/bash - 
#=============================================================================
# 
# 
#=============================================================================

#PERF=/home/mli55/linux-3.19/tools/perf/perf stat
#EVENT=LLC-store-misses, LLC-stores
#EVENT=cycles,instructions,LLC-store-misses,LLC-stores,LLC-load-misses,LLC-loads
OUTPUT0=stat.txt
#OUTPUT1=perf_stat1.txt
#OUTPUT2=perf_stat2.txt

#CMD0="./rcn_frdly_bp -s 1048576 -l 2097152 -e 400"
#CMD1="./rcn_frdly_cc -s 1048576 -l 2097152 -e 400"
#CMD2="./warmup_bp -s 1048576 -l 2097152 -e 400"
#CMD3="./warmup_cc -s 1048576 -l 2097152 -e 400"
TIMES=10

rm $OUTPUT0

for  ((i = 0; i < $TIMES; i++))
do
  #$CMD0 >> $OUTPUT0
  #$CMD1 >> $OUTPUT0
  /home/mli55/linux-3.19/tools/perf/perf stat -e  LLC-store-misses,LLC-stores ./rcn_frdly_bp -s 1048576 -l 2097152 -e 400 2>> $OUTPUT0
  #$PERF -e $EVENT $CMD1 2>> $OUTPUT0
  #$PERF -e $EVENT $CMD2 2>> $OUTPUT0
  #$PERF -e $EVENT $CMD3 2>> $OUTPUT0

done


#./calculator $OUTPUT0 > $OUTPUT2

#./calculator $OUTPUT1 >> $OUTPUT2

#./mean_IPC $OUTPUT2

#rm $OUTPUT0 $OUTPUT1 $OUTPUT2
