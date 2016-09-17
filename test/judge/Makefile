CC=g++
CFLAGS=-O2 -std=c++11
SRC=../../source
INCLUDES=-I$(SRC)/engine -I$(SRC)/global

all: test_ui

test_ui: test_ui.o judge_20M.o player_cards.o community.o card.o error.o logger.o sets.o
	$(CC) $(CFLAGS) $(INCLUDES) -o test_ui test_ui.o judge_20M.o player_cards.o community.o card.o error.o logger.o sets.o
				

test_ui.o: test_ui.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c test_ui.cpp

judge_20M.o: $(SRC)/engine/judge_20M.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/judge_20M.cpp

player_cards.o: $(SRC)/engine/player_cards.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/player_cards.cpp

community.o: $(SRC)/engine/community.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/community.cpp

card.o: $(SRC)/global/card.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/global/card.cpp

error.o: $(SRC)/global/error.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/global/error.cpp

logger.o: $(SRC)/global/logger.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/global/logger.cpp

sets.o: $(SRC)/global/sets.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/global/sets.cpp

