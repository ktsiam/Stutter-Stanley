CXX      = g++
CXXFLAGS = -g -std=c++14 -Wall -Wextra -Wconversion #-Weverything

stanley: expression.o value.o parser.o main.o 
	${CXX}  -o stanley   expression.o value.o parser.o main.o

expression.o: base.hpp value.hpp expression.hpp expression.cpp
	${CXX} ${CXXFLAGS} -c expression.cpp

value.o: base.hpp value.hpp value.cpp
	${CXX} ${CXXFLAGS} -c value.cpp

parser.o: base.hpp value.hpp expression.hpp parser.hpp parser.cpp
	${CXX} ${CXXFLAGS} -c parser.cpp

main.o: main.cpp parser.hpp
	${CXX} ${CXXFLAGS} -c main.cpp

clean:
	rm -f stanley
	rm -f *.o
