INCLUDE	=	-I ../inc
CXXFLAGS+=	${INCLUDE}
OBJECTS	=	cgi.o \
			cgi_impl.o \
			cookie.o \
			header.o

default: libcgixx.a

libcgixx.a: ${OBJECTS}
	${AR} ${ARFLAGS} $@ ${OBJECTS}
	cp -p $@ ../lib/

clean:
	rm -f core *.core ${OBJECTS} libcgixx.a

.SUFFIXES: .cxx
.cxx.o:
	${CXX} ${CXXFLAGS} -o $@ -c $<
