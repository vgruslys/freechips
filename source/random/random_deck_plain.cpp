#include "random_deck_plain.h"
#include <iostream>
using namespace std;
RandomDeckPlain::RandomDeckPlain()
{
    _cards.reserve(52);
    _card_position.reserve(52);
    initialize();
}

void RandomDeckPlain::initialize()
{
    for(int i=0; i!= 52; i++)
    {
        _cards[i] = i;
        _card_position[i] = i;
    }
    _end = 52;
}

Card RandomDeckPlain::drawCard()
{
    if(_end == 0)
        throw RandomError("No card to draw from\n");
    
    int pos = _rgen.randInt(0, _end-1);
    remove(pos);
    return intToCard(_cards[_end]); 
}

void RandomDeckPlain::remove(int pos)
{
    _end--;
    swap(pos, _end);

}

void RandomDeckPlain::returnCard()
{
    if(_end == 52)
        throw RandomError("No card to return.\n");
        
    _end++;
}
void RandomDeckPlain::swap(int i, int j)
{
    _card_position[_cards[i]] = j;
    _card_position[_cards[j]] = i;
    int temp = _cards[i];
    _cards[i] = _cards[j];
    _cards[j] = temp;
}

void RandomDeckPlain::removeCard(Card card)
{
    int num = cardToInt(card);
    if(_card_position[num] < _end)
        throw RandomError("Card is not in the deck\n");
    swap(_card_position[num], _end-1);
    _end--;
}

void RandomDeckPlain::reset()
{
    _end = 52;
}

void RandomDeckPlain::addCard(Card card)
{
    int num = cardToInt(card);
    if(_card_position[num] >= _end)
        throw RandomError("Card is already in the deck\n");

    swap(_card_position[num], _end);
    _end++;
}

void RandomDeckPlain::drawCard(Card* array, int n)
{
    for(int i=0; i!=n; i++)
    {
        array[i] = drawCard();
    }
}

void RandomDeckPlain::returnCard(int n) {
    for(int i=0; i!=n; i++)
        returnCard();
}