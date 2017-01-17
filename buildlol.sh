#!/bin/bash

 g++ -I/usr/local/include/ -L /usr/local/lib/ -std=c++11 main.cpp -o particles -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -framework OpenGL -framework GLUT -framework OpenGL -framework Cocoa;

if [ "$?" -eq "0" ]; then
	./particles $0
fi
