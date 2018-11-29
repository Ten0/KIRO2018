CXX=clang++
CXXFLAGS=-std=c++11 -g -Wall -Wextra -DDEBUG -O3

all: 11 21 31 41 51 61 71 81
11:
21:
31:
41:
51:
61:
71:
81:

greedyChemins:
pim.out: greedyChemins
	./greedyChemins pim
grenoble.out: greedyChemins
	./greedyChemins grenoble
nice.out: greedyChemins
	./greedyChemins nice
submissions: greedyChemins
	./greedyChemins grenoble &
	./greedyChemins nice &
	./greedyChemins pim
v: greedyChemins
	valgrind ./greedyChemins grenoble
