BMARK = hashtable

TARGET_REDO_BASE      = $(BMARK)-redo-base
TARGET_UNDO_BASE      = $(BMARK)-undo-base
TARGET_REDO_CLWB_BASE = $(BMARK)-redo-clwb-base
TARGET_UNDO_CLWB_BASE = $(BMARK)-undo-clwb-base
TARGET_WAL            = $(BMARK)-wal
TARGET_FWB            = $(BMARK)-fwb

TARGET = $(TARGET_REDO_BASE) $(TARGET_UNDO_BASE) $(TARGET_REDO_CLWB_BASE) $(TARGET_UNDO_CLWB_BASE) $(TARGET_WAL) $(TARGET_FWB)              

OBJS_REDO_BASE      = $(TARGET_REDO_BASE).o
OBJS_UNDO_BASE      = $(TARGET_UNDO_BASE).o
OBJS_REDO_CLWB_BASE = $(TARGET_REDO_CLWB_BASE).o
OBJS_UNDO_CLWB_BASE = $(TARGET_UNDO_CLWB_BASE).o
OBJS_WAL            = $(TARGET_WAL).o
OBJS_FWB            = $(TARGET_FWB).o

OBJS = $(OBJS_REDO_BASE) $(OBJS_UNDO_BASE) $(OBJS_REDO_CLWB_BASE) $(OBJS_UNDO_CLWB_BASE) $(OBJS_WAL) $(OBJS_FWB)              
#CFLAGS = -O0 -g -ggdb -w  -Wall -std=c++11
CFLAGS = -O3 -w -g -fno-builtin -std=c++11
#LDFLAGS = -lpthread 
LDFLAGS = /home/mogleari/research/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = /home/mogleari/research/pin-2.11/source/tools/Apps/mcsim_wrapper.o
CC = g++ 

all: $(TARGET) 

.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS_REDO_BASE):      DFLAGS = -DBASELINE -DREDO 
$(OBJS_UNDO_BASE):      DFLAGS = -DBASELINE -DUNDO
$(OBJS_REDO_CLWB_BASE): DFLAGS = -DBASELINE -DREDO -DCLWB
$(OBJS_UNDO_CLWB_BASE): DFLAGS = -DBASELINE -DUNDO -DCLWB
$(OBJS_WAL):            DFLAGS = -DCLWB
$(OBJS_FWB):            DFLAGS = 

$(OBJS): %.o: $(BMARK).cc
	$(CC) -c $(CFLAGS) $(DFLAGS) -c $< -o $@

$(TARGET_REDO_BASE):      DFLAGS = -DBASELINE -DREDO 
$(TARGET_UNDO_BASE):      DFLAGS = -DBASELINE -DUNDO
$(TARGET_REDO_CLWB_BASE): DFLAGS = -DBASELINE -DREDO -DCLWB
$(TARGET_UNDO_CLWB_BASE): DFLAGS = -DBASELINE -DUNDO -DCLWB
$(TARGET_WAL):            DFLAGS = -DCLWB
$(TARGET_FWB):            DFLAGS = 

$(TARGET_REDO_BASE):      OBJ = $(OBJS_REDO_BASE)    
$(TARGET_UNDO_BASE):      OBJ = $(OBJS_UNDO_BASE)    
$(TARGET_REDO_CLWB_BASE): OBJ = $(OBJS_REDO_CLWB_BASE)
$(TARGET_UNDO_CLWB_BASE): OBJ = $(OBJS_UNDO_CLWB_BASE)
$(TARGET_WAL):            OBJ = $(OBJS_WAL)     
$(TARGET_FWB):            OBJ = $(OBJS_FWB)     

$(TARGET): $(OBJS) $(WRAPPER_OBJ)
	$(CC) -static $(OBJ) $(WRAPPER_OBJ) $(CFLAGS) $(DFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f *.o $(TARGET) $(OBJS) pin*.log
