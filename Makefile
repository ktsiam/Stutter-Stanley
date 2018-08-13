CXX      = g++
CXXFLAGS = -g -std=c++14 -Wall -Wextra -Wconversion #-Weverything

stanley: exp.o main.o 
	${CXX}  -o stanley   exp.o main.o

exp.o: exp.hpp exp.cpp
	${CXX} ${CXXFLAGS} -c exp.cpp

main.o: main.cpp parser.hpp
	${CXX} ${CXXFLAGS} -c main.cpp

clean:
	rm -f stanley
	rm -f *.o
