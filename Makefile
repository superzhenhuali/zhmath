CPP=g++
CXX=g++
CC=gcc
CFLAGS=-g -Wall 
CINCLUDES=

LIBNAME=zhmath
LDEXEFLAG=-o
LDFLAGS= -L. -l$(LIBNAME)
LIBS=
TLIBA=lib$(LIBNAME).a

HEADERS = 	\
		utils.h  \
		logs.h \
		math_list.h \
		bigint.h  \
		matrix.h  \
		quiver.h \
		algebra.h \
		polynomial.h \
		equation.h \
		heckecliff.h \
		testing.h \


SOURCES_LIBS =	\
		utils.c \
		logs.c \
		math_list.c \
		bigint.c \
 		matrix.c \
		quiver.c \
		algebra.c \
		polynomial.c \
		equation.c \
		heckecliff.c \
		testing.c \


SOURCES = $(SOURCES_LIBS)
OBJS := $(SOURCES:.c=.o)

#.cpp.o: $(HEADERS)
#	$(CXX) -c $*.cpp $(CFLAGS) -o $*.o

%.o : %.c
	$(CC)  -c  $(CFLAGS) $^

all:test $(TLIBA)

$(TLIBA):$(OBJS)
	ar cr $(TLIBA) $(OBJS)

test:test.o $(TLIBA)
	$(CC)  $(CFLAGS)  test.c $(LDFLAGS) $(LIBS) -o test

test.o:test.c
	$(CC)  -c $(CFLAGS) $< 

clean:
	rm -rf *.a *.o test
