SRC = clean_cache
TARGET_UNDO = $(SRC)
OBJS_UNDO = $(TARGET_UNDO).o
#CFLAGS = -O0 -g -ggdb -w  -Wall
CFLAGS = -O3 -w -g -fno-builtin -std=c++11
#LDFLAGS = -lpthread 
LDFLAGS = /home/mli55/mcsim/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = /home/mli55/mcsim/pin-2.11/source/tools/Apps/mcsim_wrapper.o
CC = g++ 

all: $(TARGET_UNDO)

$(TARGET_UNDO): $(OBJS_UNDO) $(WRAPPER_OBJ)
	$(CC) -DPERSISTENT -DUNDOLOG -static $(OBJS_UNDO) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET_UNDO) $(LDFLAGS)

.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS_UNDO): %.o: $(SRC).cpp
	$(CC) -DPERSISTENT -DUNDOLOG -c $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OBJS_UNDO) $(TARGET_UNDO) pin*.log
