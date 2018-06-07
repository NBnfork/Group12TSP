# Parts of this file were taken for the makefile lecture in Module 1 of CS162 Fall 2017
# Parts were also used from the example written by TA Harlan James. 

#target: dependencies
#	rule to build

PROJ = TSPChristo

ASSIGN = TSPProject

SRCS = ${wildcard *.cpp}

OBJS = ${SRCS:.cpp=.o}

CXX = g++

CXXFLAGS = -std=c++0x
CXXFLAGS += -Wall
CXXFLAGS += -pedantic-errors
CXXFLAGS += -g

VALGRINDOPT = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes

${PROJ}: ${OBJS} 
	${CXX} ${CXXFLAGS} ${OBJS} -o ${PROJ}
${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c ${SRCS} 


.PHONY: clean zip debug

debug: ${PROJ}
	valgrind ${VALGRINDOPT} ./${PROJ} wrestler.txt
	 
clean: 
	@echo "RM                *o"
	@echo "RM 		${PROJ}"
	@rm -f *.o ${PROJ}

zip:
	zip ${ASSIGN}_CS325__Group_12 *.cpp *.hpp  makefile README


