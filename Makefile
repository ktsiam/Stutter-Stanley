CXX      = g++
CXXFLAGS = -g -std=c++14 -Wall -Wextra -Wconversion #-Weverything

stanley: expression.o parser.o main.o 
	${CXX}  -o stanley   expression.o parser.o main.o

stutter: expression.o altparser.o main.o 
	${CXX}  -o stanley   expression.o altparser.o main.o

expression.o: expression.hpp expression.cpp
	${CXX} ${CXXFLAGS} -c expression.cpp

parser.o: parser.hpp parser.cpp
	${CXX} ${CXXFLAGS} -c parser.cpp

main.o: main.cpp parser.hpp
	${CXX} ${CXXFLAGS} -c main.cpp

altparser.o: altparser.hpp altparser.cpp
	${CXX} ${CXXFLAGS} -c altparser.cpp

clean:
	rm -f stanley
	rm -f *.o
