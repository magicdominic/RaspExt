CC            = gcc
CXX           = g++
LINK          = g++
CFLAGS        = -Wall -c
CXXFLAGS      = -pipe -g -Wall -W
LDFLAGS       = 

all: bcm_del

bcm_del.o: bcm_del.c
	${CC} $^ ${CFLAGS} -o $@

bcm_del: bcm_del.o
	${CC} $^ ${LDFLAGS} -o $@
