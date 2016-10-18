
MKFILE      = Makefile

COMPILECPP  = g++ -g -O0 -Wall -Wextra -rdynamic -std=gnu++11

CPPSOURCE   = calc.cpp
EXECBIN     = mean

${EXECBIN} : ${CPPSOURCE}
	${COMPILECPP} -o $@ ${CPPSOURCE}

clean: 
	-rm ${EXECBIN}
