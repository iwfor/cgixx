LIBS=		-L../lib -lcgixx
CXXFLAGS=	-O2 -pipe -Wall -I../inc

default: all

all: test1

test1: test1.o
	${CXX} -o $@ test1.o ${CXXFLAGS} ${LIBS}

clean:
	rm -f core *.core *.o dump test1 buffertest objtest

.SUFFIXES: .cxx
.cxx.o:
	${CXX} ${CXXFLAGS} -o $@ -c $<
