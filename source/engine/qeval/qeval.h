#ifndef __QEVAL__
#define __QEVAL__

#include "stdint.h"

/***************************************************************************************************************
 * 
 * QEval - Abstract interface for QEval evaluators 
 * 
 * There are two functions:
 * 
 *	uint32_t score(uint64 key) const
 *		This function takes in as argument a key and returns a score.
 * 		A key is a 64bit unsigned integer representing a set of 7 cards. The correct format
 * 		for such a key is as follows:
 * 
 * 		key[0:12] represents hearts; 
 *		key[13:25] represents diamonds;
 *		key[26:38] represents clubs;
 * 		key[39:51] represents spades;
 * 		key[52:54] is the number of hearts;
 * 		key[55:57] is the number of diamonds;
 * 		key[58:60] is the number of clubs;
 * 		key[61:63] is the number of spades.
 * 
 * 		For the first 4 sections of 13 bits, the ith bit is set if that height in the given suit is present 
 * 		amongst the seven cards, otherwise it is not set. As for the last four, they rare the actual number
 * 		of cards of the corresponding suit present in the hand. 
 * 
 * 		The format of the returned score is as follows:
 * 		
 * 		score[0:12]  low kickers
 * 		score[13:25] high kickers
 * 		score[26:29] combination type
 *		
 * 		(The combination type is a number between 0 and 8 representing the type of the combination; 
 * 		0 = High Cards, 1 = Pair, 2 = Two pairs,..., 8 = Straight flush)
 * 		
 * 		For example, a Full House consisting of 3 aces and two fours would be represented as
 * 		110 1000000000000 0000000000100 
 * 
 * 	void score(uint64_t* keys, uint32_t* scores, uint64_t size) const
 * 		This function takes in as arguments a pointer to an array of 64bit keys, a pointer to an array of 
 * 		32bit integers to store the computed scores, and the number of scores to be computed. This function 
 * 		should be used whenever a batch processing of the hands is possible, for example in Monte Carlo 
 * 		simulations. 
 *
 **************************************************************************************************************/

class QEval {
	public:
		virtual uint32_t score(uint64_t) const = 0;
		virtual void score(uint64_t*, uint32_t*,uint64_t) const = 0;
		
};
#endif
