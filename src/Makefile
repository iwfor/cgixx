INCLUDE	=	-I ../inc
OBJECTS	=	cgi.o \
			cgi_impl.o \
			cookie.o \
			header.o

default: libcgixx

libcgixx: ${OBJECTS}
	${AR} ${ARFLAGS} libcgixx.a ${OBJECTS}
	cp -p libcgixx.a ../lib/

clean:
	rm -f core *.core ${OBJECTS} libcgixx.a
