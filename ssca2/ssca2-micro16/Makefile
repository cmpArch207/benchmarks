include Makefile.var

TARGET = SSCA2
TARGET_REDO = $(TARGET)-pm-redo
TARGET_UNDO = $(TARGET)-pm-undo

OBJS = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph.o getStartLists.o findSubGraphs.o  \
    betweennessCentrality.o

OBJS_REDO = SSCA2-pm-redo.o init-pm-redo.o utils-pm-redo.o genScalData-pm-redo.o gen2DTorus-pm-redo.o \
    computeGraph-pm-redo.o getStartLists-pm-redo.o findSubGraphs-pm-redo.o  \
    betweennessCentrality-pm-redo.o

OBJS_UNDO = SSCA2-pm-undo.o init-pm-undo.o utils-pm-undo.o genScalData-pm-undo.o gen2DTorus-pm-undo.o \
    computeGraph-pm-undo.o getStartLists-pm-undo.o findSubGraphs-pm-undo.o  \
    betweennessCentrality-pm-undo.o

LDFLAGS = /home/mogleari/research/pin-2.11/source/tools/Pthread/libmypthread.a
WRAPPER_OBJ = /home/mogleari/research/pin-2.11/source/tools/Apps/mcsim_wrapper.o

all: $(TARGET) $(TARGET_REDO) $(TARGET_UNDO)

.c.o: defs.h Makefile
	$(CC) $(INC) $(CFLAGS) -c $<

$(TARGET): sprng $(OBJS) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) $(INC) $(OBJS) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET) $(LIB) $(LDFLAGS)

$(TARGET_REDO): sprng $(OBJS_REDO) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) -DPERSISTENT -DREDOLOG $(INC) $(OBJS_REDO) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET_REDO) $(LIB) $(LDFLAGS)

$(TARGET_UNDO): sprng $(OBJS_UNDO) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) -DPERSISTENT -DUNDOLOG $(INC) $(OBJS_UNDO) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET_UNDO) $(LIB) $(LDFLAGS)

sprng: 
	(cd sprng2.0; $(MAKE); cd ..)

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INC) -c $< -o $@

$(OBJS_REDO): %-pm-redo.o: %.c
	$(CC) -DPERSISTENT -DREDOLOG -c $(CFLAGS) $(INC) -c $< -o $@

$(OBJS_UNDO): %-pm-undo.o: %.c
	$(CC) -DPERSISTENT -DUNDOLOG -c $(CFLAGS) $(INC) -c $< -o $@

cleansprng: 
	(cd sprng2.0; $(MAKE) clean; cd ..)

clean: cleansprng
	rm -f *.o *~ $(TARGET) $(TARGET_REDO) $(TARGET_UNDO) core*
