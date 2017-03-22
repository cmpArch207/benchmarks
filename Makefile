.PHONY: all clean

all: 
	-$(MAKE) -C hashtable
	-$(MAKE) -C rbtree
	-$(MAKE) -C sps 
	-$(MAKE) -C btree 
	-$(MAKE) -C sixdegree
	-$(MAKE) -C ssca2

clean: 
	-$(MAKE) -C hashtable clean
	-$(MAKE) -C rbtree clean
	-$(MAKE) -C sps clean
	-$(MAKE) -C btree clean
	-$(MAKE) -C sixdegree clean
	-$(MAKE) -C ssca2 clean

