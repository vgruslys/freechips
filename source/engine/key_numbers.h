#ifndef __KEY_NUMBERS__
#define __KEY_NUMBERS__
#include "stdint.h"
/*Static arrays containing the key numbers used by the Community and Judge */
static const uint64_t suit_numbers [4]= {6,38,35,1}; // these 4 numbers have the property that the sum of any 5, 6, or 7 of them 
											// with repeats allowed give a distinct sum
static const uint64_t height_numbers [13] = {192812, 149924,36127,194147 ,65730,195103,174299,
										30521,185463,52988,90749,19256,65913};
										//These numbers have the property that the sum of any 5 of them with repeats allowed 
										 // give a distinct sum.
static const uint64_t hand_height_numbers [13] = {6,164,189,216,146,43,212,15,172,82,196,9,115};
										// These numbers have the property that the sum of any two of them with repeats allowed is
										// distinct. They are used to represent an unsuited player hand 
										// The alternative would be to use an 8 bit integer where 4 bits represent the height of a card; but
										// the drawback of that is - harder to compute than just an addition
										// notice that the max sum of two of the above numbers is about 400 so it is not much worse than 
										// 256 numbers as given by the 8bit method
										
/*The following numbers are used when generating tables: they will be the size of the corresp table */
static const int MAX_SUIT_NUMBERS_5SUM = 240; //5*the max value of suit_numbers
static const int MAX_HEIGHT_NUMBERS_5SUM = 975515; //ditto height_numbers
static const int MAX_HAND_HEIGHT_NUMBERS_2SUM = 432; //ditto hand_height_numbers
										
#endif