#include<iostream>
#include "card.h"
#include "random_deck_plain.h"
#include "odds_calculator_random.h"
#include "judge_table.h"
using namespace std;

int main()
{
	/*
	RandomDeckPlain deck;
	/*
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
	
	*/

	/*
	OddsCalculatorRandom calculator;
	Card mycards[2];
	Card oppcards[2];
	Card street[5];
	

	parseCards("[As,Ah]",mycards);
	parseCards("[Kd,Kc]", oppcards);
	parseCards("[4s,4h,4d,4c,5c]", street);
	
	pair<float, float> result = calculator.handOdds(mycards[0], mycards[1], oppcards[0], oppcards[1], street, 5);
	cout << result.first << " " << result.second << " " << result.first + 0.5*result.second  << endl;
	*/
	JudgeTable judge;
	return 0;
	
}
