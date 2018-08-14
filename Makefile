CXX      = g++
CXXFLAGS = -g -std=c++14 -Wall -Wextra -Wconversion #-Weverything

stanley: expression.o parser.o main.o 
	${CXX}  -o stanley   expression.o parser.o main.o

expression.o: expression.hpp expression.cpp
	${CXX} ${CXXFLAGS} -c expression.cpp

parser.o: parser.hpp parser.cpp
	${CXX} ${CXXFLAGS} -c parser.cpp

main.o: main.cpp parser.hpp
	${CXX} ${CXXFLAGS} -c main.cpp

clean:
	rm -f stanley
	rm -f *.o
