#ifndef __RDPLAIN__
#define __RDPLAIN__
#include "random_deck.h"
#include "rplain.h"
#include "card.h"
#include <vector>
class RDPlain: public RandomDeck {
    public:
    
        void removeCard(Card); //Removes a card from the deck
        void addCard(Card); //Adds a card to the deck
        void returnCard(); // Returns the last card
        void reset(); //Adds all the missing cards back to the deck
        Card drawCard(); //Draws a card from the deck
        void drawCard(Card*, int); //Draws n cards from the deck and stores them in an array of cards
        
        RDPlain();
    private:
        std::vector<int> _cards; //array of 52 cards
        std::vector<int> _card_position; // ith entry points to the poition of the ith card in _cards
        int _end; //points to one past the last available card
        void remove(int); //puts the ith card at position _end-1 and decreases _end by one
        void swap(int i, int j); //swaps card at pos i and pos j
        void initialize(); //intialize the deck
        RPlain _rgen;
};
#endif
