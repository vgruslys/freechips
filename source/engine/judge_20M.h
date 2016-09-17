#ifndef __JUDGE_TABLE__
#define __JUDGE_TABLE__

#include "community.h"
#include "player_cards.h"
#include "judge.h"

class Judge20M /*: public Judge*/ {
public:
	int verdict(const Community&, const PlayerCards&) const; // the judge renders its verdict, which is an int 
														// representing the value of the best combination
	Judge20M();
	~Judge20M();

private:

	int flushScore(uint64_t) const;     // to generate the flush table, the judge uses this to find the flush score
	int unsuitedScore(uint64_t) const;  //to generate the unsuited table, the judge uses this

    uint64_t* _five_highest_cards;      // This array is used extract the five highest 'ON' bits from a 13bit integer.
                                        // This array has to be precomputed by the constructor.

    uint8_t* _highest_straight;         // This array is used to extract the highest 'straight' from a 13bit integer.
                                        // If fewer than 5 bits are 'ON', then the value is 0.
                                        // If at least 5 bits are 'ON', but there is no 'straight', then the value is 0x10.
                                        // If there is a straight, then the value is 0x1?, where ? is the heighest card in that straight (an int from {2,3,...,14}).
                                        // This array has to be precomputed by the constructor.

    void generateFiveHighestCards();    // Precompute the _five_highest_cards array. Called by the constructor.
    void generateHighestStraight();     // Precompute the _highest_straight array. Called by the constructor.
};
#endif
