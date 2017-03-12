main: main.cpp cat.o ln.o
	g++ main.cpp cat.o ln.o
cat.o: cat.cpp cat.h 
	g++ -c cat.cpp
ln.o: ln.cpp ln.hpp
	g++ -c ln.cpp