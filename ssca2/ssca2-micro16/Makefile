include Makefile.var

TARGET = SSCA2
TARGET_REDO_BASE      = $(TARGET)-redo-base
TARGET_UNDO_BASE      = $(TARGET)-undo-base
TARGET_REDO_CLWB_BASE = $(TARGET)-redo-clwb-base
TARGET_UNDO_CLWB_BASE = $(TARGET)-undo-clwb-base
TARGET_WAL            = $(TARGET)-wal
TARGET_FWB            = $(TARGET)-fwb

OBJS = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph.o getStartLists.o findSubGraphs.o  \
    betweennessCentrality.o

OBJS_REDO_BASE = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph-redo-base.o getStartLists-redo-base.o findSubGraphs-redo-base.o  \
    betweennessCentrality-redo-base.o

OBJS_UNDO_BASE = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph-undo-base.o getStartLists-undo-base.o findSubGraphs-undo-base.o  \
    betweennessCentrality-undo-base.o

OBJS_REDO_CLWB_BASE = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph-redo-clwb-base.o getStartLists-redo-clwb-base.o findSubGraphs-redo-clwb-base.o  \
    betweennessCentrality-redo-clwb-base.o

OBJS_UNDO_CLWB_BASE = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph-undo-clwb-base.o getStartLists-undo-clwb-base.o findSubGraphs-undo-clwb-base.o  \
    betweennessCentrality-undo-clwb-base.o

OBJS_WAL = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph-wal.o getStartLists-wal.o findSubGraphs-wal.o  \
    betweennessCentrality-wal.o

OBJS_FWB = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph-fwb.o getStartLists-fwb.o findSubGraphs-fwb.o  \
    betweennessCentrality-fwb.o

LDFLAGS = /home/mogleari/research/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = /home/mogleari/research/pin-2.11/source/tools/Apps/mcsim_wrapper.o

all: $(TARGET_REDO_BASE) $(TARGET_UNDO_BASE) $(TARGET_REDO_CLWB_BASE) $(TARGET_UNDO_CLWB_BASE) $(TARGET_WAL) $(TARGET_FWB)

.c.o: defs.h Makefile
	$(CC) $(INC) $(CFLAGS) -c $<

$(TARGET): sprng $(OBJS) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) $(INC) $(OBJS) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET) $(LIB) $(LDFLAGS)

$(TARGET_REDO_BASE): sprng $(OBJS_REDO_BASE) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) -DBASELINE -DREDOLOG $(INC) $(OBJS_REDO_BASE) $(WRAPPER_OBJ) $(CFLAGS)  -o $(TARGET_REDO_BASE) $(LIB) $(LDFLAGS)

$(TARGET_UNDO_BASE): sprng $(OBJS_UNDO_BASE) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) -DBASELINE -DUNDOLOG $(INC) $(OBJS_UNDO_BASE) $(WRAPPER_OBJ) $(CFLAGS)  -o $(TARGET_UNDO_BASE) $(LIB) $(LDFLAGS)

$(TARGET_REDO_CLWB_BASE): sprng $(OBJS_REDO_CLWB_BASE) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) -DBASELINE -DREDOLOG -DCLWB $(INC) $(OBJS_REDO_CLWB_BASE) $(WRAPPER_OBJ) $(CFLAGS)  -o $(TARGET_REDO_CLWB_BASE) $(LIB) $(LDFLAGS)

$(TARGET_UNDO_CLWB_BASE): sprng $(OBJS_UNDO_CLWB_BASE) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) -DBASELINE -DUNDOLOG -DCLWB $(INC) $(OBJS_UNDO_CLWB_BASE) $(WRAPPER_OBJ) $(CFLAGS)  -o $(TARGET_UNDO_CLWB_BASE) $(LIB) $(LDFLAGS)

$(TARGET_WAL): sprng $(OBJS_WAL) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) -DCLWB $(INC) $(OBJS_WAL) $(WRAPPER_OBJ) $(CFLAGS)  -o $(TARGET_WAL) $(LIB) $(LDFLAGS)

$(TARGET_FWB): sprng $(OBJS_FWB) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) $(INC) $(OBJS_FWB) $(WRAPPER_OBJ) $(CFLAGS)  -o $(TARGET_FWB) $(LIB) $(LDFLAGS)

sprng: 
	(cd sprng2.0; $(MAKE); cd ..)

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INC) -c $< -o $@

$(OBJS_REDO_BASE): %-redo-base.o: %.c 
	$(CC) -DBASELINE -DREDOLOG $(INC) $(CFLAGS) -c $< -o $@ 

$(OBJS_UNDO_BASE): %-undo-base.o: %.c
	$(CC) -DBASELINE -DUNDOLOG $(INC) $(CFLAGS) -c $< -o $@

$(OBJS_REDO_CLWB_BASE): %-redo-clwb-base.o: %.c
	$(CC) -DBASELINE -DREDOLOG -DCLWB $(INC) $(CFLAGS) -c $< -o $@

$(OBJS_UNDO_CLWB_BASE): %-undo-clwb-base.o: %.c
	$(CC) -DBASELINE -DUNDOLOG -DCLWB $(INC) $(CFLAGS) -c $< -o $@

$(OBJS_WAL): %-wal.o: %.c
	$(CC) -DCLWB $(INC) $(CFLAGS) -c $< -o $@

$(OBJS_FWB): %-fwb.o: %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

cleansprng: 
	(cd sprng2.0; $(MAKE) clean; cd ..)

clean: cleansprng
	rm -f *.o *~ core* $(TARGET_REDO_BASE) $(TARGET_UNDO_BASE) $(TARGET_REDO_CLWB_BASE) $(TARGET_UNDO_CLWB_BASE) $(TARGET_WAL) $(TARGET_FWB)
