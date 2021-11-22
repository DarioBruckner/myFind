#Makefile for myfind
all: myfind

myfind: myFind.cpp
	g++ -g -fPIC -Wall -o myfind myFind.cpp

clean:
	rm -f myfind
