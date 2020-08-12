
# Windows makefile
CC=cl.exe
CFLAGS=/O2 /Z7 /EHsc -std:c++17
#CC=clang++.exe
#CFLAGS=-ggdb3 -std=c++17

all: mst_prim.exe shortest_path.exe connected_region.exe convex_hull.exe second_to_last_delete.exe test.exe charstats.exe locale-win.exe

%.exe: %.obj
	$(CC) /Fe:$@ $^ /debug /link

%.obj: %.cpp
	$(CC) $(CFLAGS) /Fo:$@ $<

clean:
	cmd /c "del *.exe *.out *.o"
