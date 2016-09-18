CC=g++
CFLAGS=-O3 -std=c++11
SRC=../../../source
INCLUDES=-I$(SRC)/engine -I$(SRC)/global -Iio
OBJ_DIR=../../obj
CPP_FILES=$(wildcard $(SRC)/engine/judge*.cpp)
OBJ_FILES=$(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: test

test:	test.o $(OBJ_DIR)/player_cards.o $(OBJ_DIR)/community.o $(OBJ_FILES)
	$(CC) $(CFLAGS) test.o $(OBJ_DIR)/player_cards.o $(OBJ_DIR)/community.o $(OBJ_FILES) -o test

test.o: test.cpp io/settings.h
	$(CC) $(CFLAGS) $(INCLUDES) test.cpp -c -o test.o

$(OBJ_DIR)/player_cards.o: $(SRC)/engine/player_cards.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/engine/player_cards.cpp -c -o $(OBJ_DIR)/player_cards.o

$(OBJ_DIR)/community.o: $(SRC)/engine/community.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC)/engine/community.cpp -c -o $(OBJ_DIR)/community.o

$(OBJ_FILES):	$(addprefix $(SRC)/engine/,$(patsubst %.o,%.cpp,$(notdir $@)))
	$(CC) $(CFLAGS) $(INCLUDES) $(addprefix $(SRC)/engine/,$(patsubst %.o,%.cpp,$(notdir $@))) -c -o $@



