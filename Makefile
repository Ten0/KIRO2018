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
pim.out: greedyChemins ./pim/loops.out
	./greedyChemins pim
grenoble.out: greedyChemins ./grenoble/loops.out
	./greedyChemins grenoble
nice.out: greedyChemins ./nice/loops.out
	./greedyChemins nice
submissions: greedyChemins ./grenoble/loops.out ./nice/loops.out ./pim/loops.out
	./greedyChemins grenoble &
	./greedyChemins nice &
	./greedyChemins pim
v: greedyChemins
	valgrind ./greedyChemins grenoble
