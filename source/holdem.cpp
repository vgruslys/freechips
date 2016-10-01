#include "error.h"
#include "global_types.h"
#include "history.h"
#include "communicator.h"
#include "logger.h"
#include "card_container.h"
#include <cstdio>
#include <iostream>
#include <utility>
#include "judge_q.h"
#include "odds_calculator_random.h"
#include "qeval_avx2.h"
using namespace std;
int main()
{
	CardContainer community;
	CardContainer p1;
	CardContainer p2;
	p1.addCard(0);
	p1.addCard(13);
	
	QEvalAVX2 scorer;
	JudgeQ judge(scorer);
	OddsCalculatorRandom calc(judge);
	pair<float, float> p = calc.odds(community, p1, p2);
	cout << p.first << " " << p.second << endl;
    return 0;
}

