#!/bin/bash - 
#=============================================================================
# 
#repeatedly run the command
# 
#=============================================================================

#CMD0="./rcn_frdly_bp -s 1048576 -l 2097152 -e 400"
WL="-s 200 -l 400 -e 10" #work load
#WL="-s 100 -l 200 -e 20" #work load
#WL="-s 50 -l 100 -e 40" #work load
#WL="-s 25 -l 50 -e 80" #work load

#benchmarks
BM0='./rcn_frdly_bp'
BM1='./rcn_frdly_cc'
BM2='./rcn_frdly_fl'
BM=$BM1
CMD="$BM $WL"

#CMD0="./rcn_frdly_bp $WL"
#CMD1="./rcn_frdly_cc $WL"
#CMD2="./rcn_frdly_fl $WL"
#CMD0="./rcn_frdly_bp -s 1048576 -l 8388608 -e 400"
#CMD1="./rcn_frdly_cc -s 1048576 -l 8388608 -e 400"
TIMES=1
#OUTPUT=stat_elt10.txt
#OUTPUT=stat.txt
OUTPUT=cc_elt10_ln.txt


#size, associativity, line size
I1='--I1=1024,4,32'
D1='--D1=1024,4,32'
LL='--LL=8192,8,64'
TOOL='--tool=cachegrind'
#TOOL='--tool=callgrind'

#================================
#run cachegrind
#================================
#rm $OUTPUT0

for  ((i = 0; i < $TIMES; i++))
do
  #valgrind $I1 $D1 $LL $TOOL $CMD0 2>  $OUTPUT
  #valgrind $I1 $D1 $LL $TOOL $CMD1 2>> $OUTPUT
  #valgrind $I1 $D1 $LL $TOOL $CMD2 2>  $OUTPUT
  valgrind $I1 $D1 $LL $TOOL $CMD 2>  $OUTPUT
  #valgrind $TOOL $OUTPUT $CMD1

done


#================================
#cg annotate
#================================

CG_OUTS=$(ls | grep "cachegrind.out.")


for OUT in $CG_OUTS
do
  echo $OUT
  cg_annotate --auto=yes $OUT >> $OUTPUT
  rm $OUT

done

