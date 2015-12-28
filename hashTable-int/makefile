TARGET = hashtable
OBJS = hashtable.o
#CFLAGS = -O0 -g -ggdb -w  -Wall
CFLAGS = -O3 -w -g -fno-builtin -std=c++0x #-DSPS_DEBUG
#LDFLAGS = -lpthread 
LDFLAGS = #/gpfs/home/juz138/work/software/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = #/gpfs/home/juz138/work/software/pin-2.11/source/tools/Apps/mcsim_wrapper.o
CC = g++ 

$(TARGET): $(OBJS) $(WRAPPER_OBJ)
	$(CC) -static $(OBJS) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET) $(LDFLAGS)

.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS): %.o: %.cc 
	$(CC) -c $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OBJS) $(TARGET) pin*.log
