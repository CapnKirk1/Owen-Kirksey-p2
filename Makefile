all: 1730ed

run: 1730ed
	./1730ed
1730ed: 1730ed.o
	g++ -Wall -lncurses -g -o 1730ed 1730ed.o
1730ed.o: 1730ed.cpp
	g++ -Wall -std=c++14 -c -lncurses -g -O0 -pedantic-errors 1730ed.cpp

clean:
	rm -f 1730ed
	rm -f 1730ed.o