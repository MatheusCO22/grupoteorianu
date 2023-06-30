#!/bin/bash
g++ -Wall -c ChiStd.cc
g++ -Wall ChiStd.o -lglobes -lgsl -lgslcblas -O3 -o ChiStd
./ChiStd
rm *.o