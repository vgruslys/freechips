CC=g++
CFLAGS=-O3
INCLUDES=

all:	generate_hands solver/solver solver/sorter

generate_hands:	generate_hands.cpp
	$(CC) $(CFLAGS) generate_hands.cpp -o generate_hands

solver/solver:	solver/ace_eval_decompress.o solver/solver.o
	$(CC) $(CFLAGS) solver/ace_eval_decompress.o solver/solver.o -o solver/solver

solver/ace_eval_decompress.o:	solver/ace_eval_decompress.cpp
	$(CC) $(CFLAGS) solver/ace_eval_decompress.cpp -c -o solver/ace_eval_decompress.o

solver/solver.o:	solver/solver.cpp
	$(CC) $(CFLAGS) solver/solver.cpp -c -o solver/solver.o

solver/sorter:	solver/sorter.cpp
	$(CC) $(CFLAGS) solver/sorter.cpp -o solver/sorter
