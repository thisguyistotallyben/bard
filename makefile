all: bard

bard: bard.o ncu.o
	g++ -o bard bard.o ncu.o -lncurses -lpanel

bard.o: bard.cpp ncu.h
	g++ -c bard.cpp -lncurses -lpanel

ncu.o: ncu.cpp ncu.h
	g++ -c ncu.cpp -lncurses -lpanel

clean:
	rm -rf bard *.o
