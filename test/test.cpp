#include "card.h"
#include "random_deck_plain.h"
#include "odds_calculator_random.h"
#include "judge_table.h"
#include <iostream>
using namespace std;

int main()
{
	JudgeTable judge;
	Community com;
	com.addCard(1);
	com.addCard(2);
	com.addCard(3);
	com.addCard(4);
	com.addCard(5);
	PlayerCards pc;
	pc.addCard(6);
	pc.addCard(7);
	cout << judge.verdict(com, pc) << endl;
	return 0;
	
}
