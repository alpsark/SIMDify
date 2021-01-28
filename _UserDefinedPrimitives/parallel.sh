#!/bin/sh
g++ -Wall -g -std=c++11  -c parallel.cpp -o obj/Debug/parallel-lin.o
g++  -o bin/Debug/Parallel-lin obj/Debug/parallel-lin.o   
exec ./bin/Debug/Parallel-lin
