#include "card.h"
class RandomDeck
{
    public:
        virtual void removeCard(Card) = 0; //Removes a card from the deck
        virtual void addCard(Card) = 0; //Adds a card to the deck
        virtual void reset() = 0; //Adds all the missing cards back to the deck
        virtual Card drawCard() = 0; //Draws a card from the deck
        virtual void drawCard(Card*, int) = 0; //Draws n cards from the deck and stores them in an array of cards
};