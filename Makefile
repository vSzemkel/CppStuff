
# Ubunty makefile
# make -f Makefile

CC=g++-9
CFLAGS=-O3 -Wall -ggdb3 -std=c++17

all: locale-linux.o charstats.o any_of.o

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm *.o
