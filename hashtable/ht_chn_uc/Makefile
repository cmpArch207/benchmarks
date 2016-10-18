MAIN        = ht_chn_p_bp.cpp
#MAIN        = ht_chn_p_cc.cpp
#MAIN        = ht_chn_np.cpp
CPPSRC      = hashtable_chn.cpp sync.cpp bigInt.cpp helper.cpp $(MAIN)
EXECBIN     = ${MAIN:.cpp=}
OBJS        = ${CPPSRC:.cpp=.o}
LDFLAGS     = /home/mli55/mcsim/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = /home/mli55/mcsim/pin-2.11/source/tools/Apps/mcsim_wrapper.o
CPP         = g++ -O3 -w -g -fno-builtin -std=c++11


all: $(EXECBIN) 

$(EXECBIN): $(OBJS) $(WRAPPER_OBJ)
	$(CPP) -DPERSISTENT -DREDOLOG -static $(OBJS) $(WRAPPER_OBJ) $(CFLAGS) -o $(EXECBIN) $(LDFLAGS)


.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS): $(CPPSRC)
	$(CPP) -DPERSISTENT -DREDOLOG -c $(CPPSRC)


clean:
	rm -f *.o $(OBJS) pin*.log *.sync

spotless: clean
	rm $(EXECBIN)
