include Makefile.var

TARGET = SSCA2

OBJS = SSCA2.o init.o utils.o genScalData.o gen2DTorus.o \
    computeGraph.o getStartLists.o findSubGraphs.o  \
    betweennessCentrality.o

LDFLAGS = /gpfs/home/juz138/work/software/pin-2.11/source/tools/Pthread/libmypthread.a                                        
WRAPPER_OBJ = /gpfs/home/juz138/work/software/pin-2.11/source/tools/Apps/mcsim_wrapper.o

.c.o: defs.h Makefile
	$(CC) $(INC) $(CFLAGS) -c $<


$(TARGET): sprng $(OBJS) $(WRAPPER_OBJ) defs.h Makefile
	$(CC) $(INC) $(OBJS) $(WRAPPER_OBJ) $(CFLAGS) -o $(TARGET) $(LIB) $(LDFLAGS)

sprng: 
	(cd sprng2.0; $(MAKE); cd ..)

$(WRAPPER_OBJ): %.o: %.c
	gcc -O0 -g -c $< -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INC) -c $< -o $@

cleansprng: 
	(cd sprng2.0; $(MAKE) clean; cd ..)

clean: cleansprng
	rm -f *.o *~ $(TARGET) core*
