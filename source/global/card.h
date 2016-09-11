#ifndef __CARD__
#define __CARD__

#include "error.h"
#include "global_types.h"

#include "stdint.h"
#include <cstdio>
#include <string>

typedef int8_t Card;
typedef int8_t Height;
typedef int8_t Suit;

Card strToCard(char*);
char heightToChar(Height);

int parseCards(char*, Card*);

Height getHeight(Card);
Suit getSuit(Card);

#endif
