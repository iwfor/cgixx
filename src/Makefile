INCLUDE		 =	-I ../inc
CXXFLAGS	+=	${INCLUDE}
OBJECTS		 =		cgi.o \
				cgi_impl.o \
				cookie.o \
				header.o
BASEDIR		 =	/usr/local

default: libcgixx.a

libcgixx.a: ${OBJECTS}
	${AR} ${ARFLAGS} $@ ${OBJECTS}
	cp -p $@ ../lib/

install: libcgixx.a
	if test ! -d ${BASEDIR} ; then \
		mkdir ${BASEDIR} \
	fi
	if test ! -d ${BASEDIR}/lib ; then \
		mkdir ${BASEDIR}/lib \
	fi
	cp -p libcgixx.a ${BASEDIR}/lib/
	if test ! -d ${BASEDIR}/include ; then \
		mkdir ${BASEDIR}/include \
	fi
	if test ! -d ${BASEDIR}/include/libtpt ; then \
		mkdir ${BASEDIR}/include/cgixx
	fi
	cp -p ../inc/cgixx/*.h ${BASEDIR}/include/cgixx/

clean:
	rm -f core *.core ${OBJECTS} libcgixx.a

.SUFFIXES: .cxx
.cxx.o:
	${CXX} ${CXXFLAGS} -o $@ -c $<
