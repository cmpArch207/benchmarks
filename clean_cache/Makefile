CC = g++ -O3 -w -g -fno-builtin -std=c++11
CFLAGS = -c -Wall -DDEBUG -g
LDFLAGS =#/home/mli55/mcsim/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = #/home/mli55/mcsim/pin-2.11/source/tools/Apps/mcsim_wrapper.o
COMMON_SOURCES = clean_cache.cpp sync.cpp helper.cpp
TARGET_SOURCES = rcn_frdly_bp.cpp
TEST_SOURCES = rcn_frdly_cc.cpp
COMMON_OBJECTS = $(COMMON_SOURCES:.cpp=.o)
	TARGET_OBJECTS = $(TARGET_SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)
	EXECUTABLE = rcn_frdly_bp
	TEST_EXECUTABLE = rcn_frdly_cc

.PHONY: all target tests

all: target tests

target: $(EXECUTABLE)

tests: $(TEST_EXECUTABLE)

$(EXECUTABLE): $(COMMON_OBJECTS) $(TARGET_OBJECTS)
	    $(CC) $(LDFLAGS) $^ -o $@

$(TEST_EXECUTABLE): $(COMMON_OBJECTS) $(TEST_OBJECTS)
	    $(CC) $(LDFLAGS) $^ -o $@

#$(WRAPPER_OBJ): %.o: %.c
	#gcc -O0 -g -c $< -o $@

.cpp.o:
	    $(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(OBJS) pin*.log *.sync $(EXECUTABLE) $(TEST_EXECUTABLE)

##MAIN           = stream.cpp
#MAIN           = rcn_frdly.cpp
#CPPSRC    = clean_cache.cpp sync.cpp helper.cpp $(MAIN)
#EXECBIN         = ${MAIN:.cpp=}
#OBJS            = ${CPPSRC:.cpp=.o}
#LDFLAGS = /home/mli55/mcsim/pin-2.11/source/tools/Pthread/libmypthread.a
#WRAPPER_OBJ = /home/mli55/mcsim/pin-2.11/source/tools/Apps/mcsim_wrapper.o
#CPP = g++ -O3 -w -g -fno-builtin -std=c++11


#all: $(EXECBIN)

#$(EXECBIN): $(OBJS) $(WRAPPER_OBJ)
	#$(CPP) -DPERSISTENT -DUNDOLOG -static $(OBJS) $(WRAPPER_OBJ) $(CFLAGS) -o $(EXECBIN) $(LDFLAGS)

#.SUFFIXES:
#.SUFFIXES:	.o .c .C .h .H

#$(WRAPPER_OBJ): %.o: %.c
	#gcc -O0 -g -c $< -o $@

#$(OBJS): $(CPPSRC)
	#$(CPP) -DPERSISTENT -DUNDOLOG -c $(CPPSRC)

#clean:
	#rm -f *.o $(OBJS) pin*.log *.sync

#spotless : clean
	#rm $(EXECBIN)
