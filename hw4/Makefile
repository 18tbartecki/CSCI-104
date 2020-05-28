all: hw4

hw4: main.cpp interpreter/interpreter.o interpreter/arithmetic.o interpreter/boolean.o interpreter/command.o 
	g++ -g -Wall $^ -o hw4 

interpreter/interpreter.o: interpreter/interpreter.h interpreter/interpreter.cpp
	g++ -g -Wall -c -std=c++11 interpreter/interpreter.cpp -o interpreter/interpreter.o

interpreter/arithmetic.o: interpreter/arithmetic.h interpreter/arithmetic.cpp
	g++ -g -Wall -c -std=c++11 interpreter/arithmetic.cpp -o interpreter/arithmetic.o

interpreter/boolean.o: interpreter/boolean.h interpreter/boolean.cpp
	g++ -g -Wall -c -std=c++11 interpreter/boolean.cpp -o interpreter/boolean.o

interpreter/command.o: interpreter/command.h interpreter/command.cpp
	g++ -g -Wall -c -std=c++11 interpreter/command.cpp -o interpreter/command.o


clean: 
	rm hw4 interpreter/interpreter.o interpreter/arithmetic.o interpreter/boolean.o interpreter/command.o 