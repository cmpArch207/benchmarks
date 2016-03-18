TARGET = sixdegs
TARGET_REDO = $(TARGET)-pm-redo
TARGET_UNDO = $(TARGET)-pm-undo
OBJS = $(TARGET).o
OBJS_REDO = $(TARGET_REDO).o
OBJS_UNDO = $(TARGET_UNDO).o
#CFLAGS = -O0 -g -ggdb -w  -Wall
CFLAGS = -O3 -w -g -fno-builtin -std=c++11
#LDFLAGS = -lpthread 
LDFLAGS = /home/mogleari/research/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = /home/mogleari/research/pin-2.11/source/tools/Apps/mcsim_wrapper.o
CC = g++ 

all: $(TARGET) $(TARGET_REDO) $(TARGET_UNDO)

$(TARGET): $(OBJS) $(WRAPPER_OBJ)
	$(CC) -static $(OBJS) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET) $(LDFLAGS)

$(TARGET_REDO): $(OBJS_REDO) $(WRAPPER_OBJ)
	$(CC) -DPERSISTENT -DREDOLOG -static $(OBJS_REDO) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET_REDO) $(LDFLAGS)

$(TARGET_UNDO): $(OBJS_UNDO) $(WRAPPER_OBJ)
	$(CC) -DPERSISTENT -DUNDOLOG -static $(OBJS_UNDO) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET_UNDO) $(LDFLAGS)

.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS): %.o: $(TARGET).cc
	$(CC) -c $(CFLAGS) -c $< -o $@

$(OBJS_REDO): %.o: $(TARGET).cc
	$(CC) -DPERSISTENT -DREDOLOG -c $(CFLAGS) -c $< -o $@

$(OBJS_UNDO): %.o: $(TARGET).cc
	$(CC) -DPERSISTENT -DUNDOLOG -c $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET) $(OBJS_REDO) $(TARGET_REDO) $(OBJS_UNDO) $(TARGET_UNDO) pin*.log
