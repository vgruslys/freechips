CC=g++
CFLAGS=-O2 -std=c++11
SRC=../../../source
INCLUDES=-I$(SRC)/engine -I$(SRC)/global

all: test

test: test.o judge_20M.o player_cards.o community.o 
	$(CC) $(CFLAGS) -o test test.o judge_20M.o player_cards.o community.o

test.o: test.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c test.cpp

judge_20M.o: $(SRC)/engine/judge_20M.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/judge_20M.cpp

player_cards.o: $(SRC)/engine/player_cards.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/player_cards.cpp

community.o: $(SRC)/engine/community.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/community.cpp


