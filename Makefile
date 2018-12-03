CXX=clang++
CXXFLAGS=-std=c++11 -g -Wall -Wextra -DDEBUG -O3

simulatedAnnealing:
pim: simulatedAnnealing ./pim/loops.out
	./simulatedAnnealing pim
grenoble: simulatedAnnealing ./grenoble/loops.out
	./simulatedAnnealing grenoble
nice: simulatedAnnealing ./nice/loops.out
	./simulatedAnnealing nice
submissions: simulatedAnnealing ./grenoble/loops.out ./nice/loops.out ./pim/loops.out
	./simulatedAnnealing grenoble &
	./simulatedAnnealing nice &
	./simulatedAnnealing pim
v: simulatedAnnealing
	valgrind ./simulatedAnnealing grenoble
