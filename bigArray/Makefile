#CC = g++ -O3 -w -g -fno-builtin -std=c++11
CC = gcc
CFLAGS = -c -Wall -O0 -g
LDFLAGS =#/home/mli55/mcsim/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = #/home/mli55/mcsim/pin-2.11/source/tools/Apps/mcsim_wrapper.o
COMMON_SOURCES = big_array.c sync.c helper.c
1st_SOURCES = rcn_frdly_bp.c
2nd_SOURCES = rcn_frdly_cc.c
3rd_SOURCES = rcn_frdly_fl.c
COMMON_OBJECTS = $(COMMON_SOURCES:.c=.o)
1st_OBJECTS = $(1st_SOURCES:.c=.o)
2nd_OBJECTS = $(2nd_SOURCES:.c=.o)
3rd_OBJECTS = $(3rd_SOURCES:.c=.o)
OBJECTS = $(1st_OBJECTS) $(2nd_OBJECTS) $(3rd_OBJECTS) $(COMMON_OBJECTS)
1st_EXECUTABLE = $(1st_SOURCES:.c=)
#rcn_frdly_bp
2nd_EXECUTABLE = $(2nd_SOURCES:.c=)
#rcn_frdly_cc
3rd_EXECUTABLE = $(3rd_SOURCES:.c=)

.PHONY: all 1st 2nd 3rd clean
.SUFFIXES: .o .c

all: 1st 2nd 3rd

1st: $(1st_EXECUTABLE)
2nd: $(2nd_EXECUTABLE)
3rd: $(3rd_EXECUTABLE)

$(1st_EXECUTABLE): $(COMMON_OBJECTS) $(1st_OBJECTS)
	$(CC) $^ -o $@

$(2nd_EXECUTABLE): $(COMMON_OBJECTS) $(2nd_OBJECTS)
	$(CC) $^ -o $@

$(3rd_EXECUTABLE): $(COMMON_OBJECTS) $(3rd_OBJECTS)
	$(CC) $^ -o $@

#.c.o:
$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f *.o $(OBJS) pin*.log *.sync $(1st_EXECUTABLE) $(2nd_EXECUTABLE) $(3rd_EXECUTABLE)

