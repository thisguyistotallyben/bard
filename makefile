all: bard

bard: bard.o bard_utils.o ncurses_utils.o
	g++ -o bard bard.o bard_utils.o ncurses_utils.o -lpanel -lncurses

bard.o: bard_utils.h ncurses_utils.h bard.cpp
	g++ -c bard.cpp -lpanel -lncurses

bard_utils.o: bard_utils.h bard_utils.cpp
	g++ -c bard_utils.cpp

ncurses_utils.o: ncurses_utils.h ncurses_utils.cpp
	g++ -c ncurses_utils.cpp -lpanel -lncurses

clean:
	rm bard.o bard_utils.o ncurses_utils.o bard
