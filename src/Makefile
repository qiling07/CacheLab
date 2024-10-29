
CXX = g++
CFLAGS = -Wall -Og -m32 -g
LFLAGS = -lm

all: main

main:
	$(CXX) $(CFLAGS) functions.cpp main.cpp cache.cpp simulated_ints.cpp $(LFLAGS)  -o cache

clean: 
	-rm -f cache

