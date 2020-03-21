CC = g++
CFLAGS = -std=c++11
LDFLAGS	= `root-config --libs`
FFFLAGS = -g -c
CCFLAGS = `root-config --cflags` -g -c

all: makeinputfile test_dst

makeinputfile:makeinputfile.cpp
			$(CC) -o makeinputfile makeinputfile.cpp $(CFLAGS)

test_dst: test_dst.o
	$(CC) $^ -o $@ $(LDFLAGS)


%.o: %.cxx
	$(CC) $^ -o $@ $(CCFLAGS)

clean:
	rm -f *.o test_dst *.d
	rm -f *.o makeinputfile *.d
