#CC = g++ -O3 -w -g -fno-builtin -std=c++11
CC = g++
CFLAGS = -c -O3 -w -g -fno-builtin -std=c++11
LDFLAGS =/home/mli55/mcsim/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = /home/mli55/mcsim/pin-2.11/source/tools/Apps/mcsim_wrapper.o
COMMON_SOURCES = helper.cpp #big_array.c sync.c helper.c
1st_SOURCES = testBp.cpp
2nd_SOURCES = testCc.cpp
COMMON_OBJECTS = $(COMMON_SOURCES:.cpp=.o)
1st_OBJECTS = $(1st_SOURCES:.cpp=.o)
2nd_OBJECTS = $(2nd_SOURCES:.cpp=.o)
OBJECTS = $(1st_OBJECTS) $(2nd_OBJECTS) $(COMMON_OBJECTS)
1st_EXECUTABLE = $(1st_SOURCES:.cpp=)
2nd_EXECUTABLE = $(2nd_SOURCES:.cpp=)

.PHONY: all 1st 2nd clean
.SUFFIXES: .o .cpp

all: 1st 2nd 

1st: $(1st_EXECUTABLE)
2nd: $(2nd_EXECUTABLE)

$(1st_EXECUTABLE): $(COMMON_OBJECTS) $(1st_OBJECTS) $(LDFLAGS) $(WRAPPER_OBJ)
	$(CC) $^ -o $@

$(2nd_EXECUTABLE): $(COMMON_OBJECTS) $(2nd_OBJECTS) $(LDFLAGS) $(WRAPPER_OBJ)
	$(CC) $^ -o $@


#.c.o:
$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f *.o $(OBJS) pin*.log *.sync $(1st_EXECUTABLE) $(2nd_EXECUTABLE) $(3rd_EXECUTABLE)
