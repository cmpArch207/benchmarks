TARGET = rbtree
OBJS = rbtree.o
#CFLAGS = -O0 -g -ggdb -w  -Wall
CFLAGS = -O3 -w -g -fno-builtin #-DRBTREE_DEBUG
#LDFLAGS = -lpthread 
LDFLAGS = /gpfs/home/juz138/work/software/pin-2.11/source/tools/Pthread/libmypthread.a -lboost_serialization  #J's pointer
WRAPPER_OBJ = /gpfs/home/juz138/work/software/pin-2.11/source/tools/Apps/mcsim_wrapper.o #J's pointer

#LDFLAGS = /home/sheng/workspace/simulator/mcsim_pm_isca/Pthread/libmypthread.a   #S's pointer
#WRAPPER_OBJ = /home/sheng/workspace/simulator/mcsim_pm_isca/Apps/mcsim_wrapper.o #S's pointer
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
	rm -f *.o *.debug $(OBJS) $(TARGET) pin*.log
