#!/bin/bash
g++ -Wall -c ProbStdMod.cc
g++ -Wall ProbStdMod.o -lglobes -lgsl -lgslcblas -O3 -o ProbModStd
./ProbModStd
rm *.o