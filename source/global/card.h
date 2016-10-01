#ifndef __CARD__
#define __CARD__

#include "error.h"
#include "global_types.h"

#include <cstdint>
#include <cstdio>

/*A card is an 8 bit integer; the first four bits represent the height; the last four the suit */
typedef std::uint8_t Card; 
typedef std::int8_t Height;
typedef std::int8_t Suit;

/*strToCard converts a string to a card
	example: strToCard("Kh"); gives the king of hearts
	
heightToChar converts a height to a char
 	example: heightToChar(14); gives 'A' for ace

suitToChar converts a Suit to a char

parseCards reads a string of the form [Kh,As,5c] and stores the corresponding 		result in an array of cards. There should be between 1 and 5 cards. 
 	example: 
 	Card cards[3]; 
 	parseCards("[Kh,As,5c]", cards);

cardToInt: converts a Card to an int in [0,51]
intToCard: converts an int in [0, 51] to a Card

getHeight and getSuit give the height and suit of a card, respectively.*/

Card strToCard(char*);
char heightToChar(Height);
char suitToChar(Suit);

int parseCards(char*, Card*);

int cardToInt(Card);
Card intToCard(int);

Height getHeight(Card);
Suit getSuit(Card);

#endif
