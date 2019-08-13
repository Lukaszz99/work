CC = g++
CFLAGS =-std=c++11
testfile: main.o testlibs.o
		$(CC) -o testfile main.o testlibs.o $(CFLAGS)
main.o:	main.cpp testlibs.hpp
		$(CC) -o main.o -c main.cpp $(CFLAGS)
testlibs.o:	testlibs.cpp testlibs.hpp
		$(CC) -o testlibs.o -c testlibs.cpp $(CFLAGS)
clean:
		rm *.o
