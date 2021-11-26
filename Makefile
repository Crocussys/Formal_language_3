all: main.o menu.o grammar.o precept.o
	g++ main.o menu.o grammar.o precept.o -o Laboratory_work_3

main.o: main.cpp
	g++ -c main.cpp

menu.o: menu.cpp
	g++ -c menu.cpp

grammar.o: grammar.cpp
	g++ -c grammar.cpp

precept.o: precept.cpp
	g++ -c precept.cpp

clean:
	rm -rf *.o
