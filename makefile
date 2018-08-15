all: tool.o main.o 
	g++ -o main main.o tool.o
tool.o: tool.cpp class.hpp
	g++ -c -O2 tool.cpp -std=c++11
main.o: main.cpp class.hpp
	g++ -c -O2 main.cpp -std=c++11

