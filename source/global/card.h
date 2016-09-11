#ifndef __CARD__
#define __CARD__

#include "error.h"
#include "global_types.h"

#include <cstdint>
#include <cstdio>

typedef std::int8_t Card;
typedef std::int8_t Height;
typedef std::int8_t Suit;

Card strToCard(char*);
char heightToChar(Height);

int parseCards(char*, Card*);

Height getHeight(Card);
Suit getSuit(Card);

#endif
