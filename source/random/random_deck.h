#include "card.h"
class RandomDeck
{
    public:
        void removeCard(Card) = 0; //Removes a card from the deck
        void addCard(Card) = 0; //Adds a card to the deck
        void reset(); //Adds all the missing cards back to the deck
        Card drawCard() = 0; //Draws a card from the deck
        void drawCard(Card*, int) = 0; //Draws n cards from the deck and stores then in an array of cards
};