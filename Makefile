
# Ubuntu makefile
# make -f Makefile

CC=clang++-9
CFLAGS=-O3 -Wall -ggdb3 -std=c++17

all: test.o mst_prim.o shortest_path.o connected_region.o convex_hull.o locale-linux.o charstats.o 

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm *.o
