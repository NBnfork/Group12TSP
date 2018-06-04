all : main 

main : main.o buildData.o
	g++ -std=c++11 main.o buildData.o -o main	

main.o : main.cpp
	g++ -std=c++11 -c main.cpp

buildData.o : buildData.cpp buildData.hpp
	g++ -std=c++11 -c buildData.cpp	

.PHONY : clean
	
clean :
	rm *.o main