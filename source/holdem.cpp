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
#include "time.h"
using namespace std;
int main()
{
	CardContainer community;
	CardContainer p1;
	CardContainer p2;
	p1.addCard(11);
	p1.addCard(24);
	
	QEvalAVX2 scorer;
	JudgeQ judge(scorer);
	OddsCalculatorRandom calc(judge);
	clock_t t1 = clock();
	pair<float, float> p = calc.odds(community, p1, p2);
	cout << p.first << " " << p.second << endl;
	clock_t t2 = clock();
	clock_t t = t2-t1;
	cout << "Time taken: " << (float) t / (float) CLOCKS_PER_SEC << endl;
    return 0;
}

