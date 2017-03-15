main: main.cpp cat.o 
	g++ main.cpp cat.o 
cat.o: cat.cpp cat.h 
	g++ -c cat.cpp