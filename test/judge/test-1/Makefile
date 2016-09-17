CC=g++
CFLAGS=-O2
SRC=../../../source
INCLUDES=-I$(SRC)/engine -I$(SRC)/global

all: test

test: test.o judge_table.o player_cards.o community.o
	$(CC) $(CFLAGS) -o test test.o judge_table.o player_cards.o community.o
				

test.o: test.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c test.cpp

judge_table.o: $(SRC)/engine/judge_table.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/judge_table.cpp

player_cards.o: $(SRC)/engine/player_cards.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/player_cards.cpp

community.o: $(SRC)/engine/community.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/community.cpp

