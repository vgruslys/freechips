CC=g++
CFLAGS=-O3 -std=c++11
SRC=../../../source
INCLUDES=-I$(SRC)/engine -I$(SRC)/global
OBJ_DIR=../../obj
CPP_FILES=$(wildcard $(SRC)/engine/judge*.cpp)
OBJ_FILES=$(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: test_ui comparator

comparator: comparator.cpp
	$(CC) $(CFLAGS) comparator.cpp -o comparator

test_ui: test_ui.o $(OBJ_DIR)/player_cards.o $(OBJ_DIR)/community.o $(OBJ_DIR)/card.o $(OBJ_DIR)/error.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/sets.o $(OBJ_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) test_ui.o $(OBJ_DIR)/player_cards.o $(OBJ_DIR)/community.o $(OBJ_DIR)/card.o \
				    $(OBJ_DIR)/error.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/sets.o $(OBJ_FILES) -o test_ui
				

test_ui.o: test_ui.cpp
	$(CC) $(CFLAGS) $(INCLUDES) test_ui.cpp -c -o test_ui.o

judge_20M.o: $(SRC)/engine/judge_20M.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC)/engine/judge_20M.cpp

$(OBJ_DIR)/player_cards.o: $(SRC)/engine/player_cards.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/engine/player_cards.cpp -c -o $@

$(OBJ_DIR)/community.o: $(SRC)/engine/community.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/engine/community.cpp -c -o $@

$(OBJ_DIR)/card.o: $(SRC)/global/card.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/global/card.cpp -c -o $@

$(OBJ_DIR)/error.o: $(SRC)/global/error.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/global/error.cpp -c -o $@

$(OBJ_DIR)/logger.o: $(SRC)/global/logger.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/global/logger.cpp -c -o $@

$(OBJ_DIR)/sets.o: $(SRC)/global/sets.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/global/sets.cpp -c -o $@

$(OBJ_FILES):	$(addprefix $(SRC)/engine/,$(patsubst %.o,%.cpp,$(notdir $@)))
	$(CC) $(CFLAGS) $(INCLUDES) $(addprefix $(SRC)/engine/,$(patsubst %.o,%.cpp,$(notdir $@))) -c -o $@

