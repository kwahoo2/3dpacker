CC=g++

CFLAGS=-c -std=c++11 -Wall -I./includes 

LDFLAGS = `pkg-config --libs glfw3` -lGL -lX11 -lpthread -lXrandr -lXi -lassimp -lm -ldl

all: glmodel 

glmodel: glmodel.o glad.o
	$(CC) glmodel.o glad.o -o glmodel $(LDFLAGS)

glad.o: glad.c
	$(CC) $(CFLAGS) glad.c
	
glmodel.o: glmodel.cpp
	$(CC) $(CFLAGS) glmodel.cpp
	

clean:
	rm *o glmodel
