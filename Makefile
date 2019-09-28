CC=g++

CFLAGS=-c -std=c++11 -Wall

all: 3dpacker 

3dpacker: 3dpacker.o vrmlimporter.o trianglecalculator.o fileexporter.o
	$(CC) 3dpacker.o vrmlimporter.o trianglecalculator.o fileexporter.o -o 3dpacker

3dpacker.o: 3dpacker.cpp
	$(CC) $(CFLAGS) 3dpacker.cpp
	
vrmlimporter.o: vrmlimporter.cpp
	$(CC) $(CFLAGS) vrmlimporter.cpp
	
trianglecalculator.o: trianglecalculator.cpp
	$(CC) $(CFLAGS) trianglecalculator.cpp
	
fileexporter.o: fileexporter.cpp
	$(CC) $(CFLAGS) fileexporter.cpp

clean:
	rm *o 3dpacker
