#!/bin/sh
g++ -Wall -g -std=c++11  -c define_instruction.cpp -o obj/Debug/define_instruction-lin.o
g++  -o bin/Debug/define_instruction-lin obj/Debug/define_instruction-lin.o 

exec ./bin/Debug/define_instruction-lin 
