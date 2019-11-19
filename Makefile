
# Ubuntu makefile
# make -f Makefile

CC=clang++-9
CFLAGS=-O3 -Wall -ggdb3 -std=c++17

all: test.o locale-linux.o charstats.o any_of.o

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm *.o
