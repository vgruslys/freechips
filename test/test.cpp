#include<iostream>
#include "card.h"
#include "rdplain.h"
using namespace std;

int main()
{
	RDPlain deck;
	Card card;
	for(int i=0; i!=10; i++)
	{
		card = deck.drawCard();
		cout << heightToChar(getHeight(card)) << suitToChar(getSuit(card)) << " " << cardToInt(card) << endl;
	}
	
	deck.reset();
	int array[52];
	for(int i=0; i!=52; i++)
		array[i] = 0;
	int MAX = 10000000;
	for(int i=0; i!=MAX; i++)
	{
		card = deck.drawCard();
		array[cardToInt(card)] ++;
		deck.returnCard();
	}
	for(int i=0; i!=52; i++)
	{
		cout << (float) array[i] / (float) MAX << endl; 
	}
	return 0;
}
