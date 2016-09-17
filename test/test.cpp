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
	
	for(int i=0; i!=1000000000; i++) {
		judge.verdict(com,pc);
	}
	/*uint64_t counter = 0;
	com.reset();
	pc = PlayerCards();
	for(int i1=0; i1!=52;i1++) {
		com.addCard(i1);
		for(int i2=i1+1; i2!=52;i2++) {
			com.addCard(i2);
			for(int i3=i2+1; i3 != 52 ; i3++) {
				com.addCard(i3);
				for(int i4 = i3+1; i4!=52; i4++) {
					com.addCard(i4);
					for(int i5 = i4+1; i5!=52; i5++) {
						com.addCard(i5);
						for(int cc1=0; cc1 !=52; cc1++) {
							pc.addCard(cc1);
							for(int cc2 = cc1 +1; cc2!=52; cc2++) {
								pc.addCard(cc2);
								if(__builtin_popcountll(com.getKey() | pc.getKey())==7) {
									judge.verdict(com,pc);
								}
								counter++;
								pc.removeCard(cc2);
							}
							pc.removeCard(cc1);
							
						}
						com.removeCard(i5);
						
					}
					com.removeCard(i4);
				}
				com.removeCard(i3);
			}
			com.removeCard(i2);
		}
		com.removeCard(i1);
	}
	cout << "Hands:" << counter << endl;*/
	return 0;
	
}
