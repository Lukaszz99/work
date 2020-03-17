CC	= g++

LDFLAGS	= `root-config --libs`
FFFLAGS = -g -c
CCFLAGS = `root-config --cflags` -g -c

all: test_dst

test_dst: test_dst.o
	$(CC) $^ -o $@ $(LDFLAGS)



%.o: %.cxx
	$(CC) $^ -o $@ $(CCFLAGS)

clean:
	rm -f *.o test_dst *.d
