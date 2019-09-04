
# Windows makefile
CC=cl.exe
CFLAGS=/O2 /EHsc -std:c++17

all: test.exe charstats.exe locale-win.exe

%.exe: %.obj
	$(CC) /Fe:$@ $^ /link

%.obj: %.cpp
	$(CC) $(CFLAGS) /Fo:$@ $<

clean:
	cmd /c "del *.exe *.out *.o"
