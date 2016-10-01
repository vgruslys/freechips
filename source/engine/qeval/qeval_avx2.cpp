#include "vectorclass.h"
#include "vector_algorithms.h"
#include "qeval_avx2.h"
#include "immintrin.h"
#include <stdexcept>

#define popc __builtin_popcountll
#define bsr __builtin_clzll
#define bsf __builtin_ctzll
#define blsr __blsr_u64


/***************************************************************************************************
 * Single hand scoring functions
 *
 * uint32_t QEvalAVX2 :: score (uint64_t key) const
 *		Uses bit manipulation to determine the frequency masks freq1, freq2, freq3, freq4
 * 		These are essentially fields of 13 bits where the jth bit of freqi is present if 
 * 		there are at least j cards of height i amongst the seven cards. 
 * 		Then if-else statements determine the best combination present amongst high cards, 
 * 		pairs, two pairs, three of a kind, full house, and four of a kind, using these masks. 
 * 		Then the presence of a straight is checked by looking up a small table (8kb).
 * 		Finally, we look for flushes and possibly straight flushes (the latter using the 
 * 		aforementioned lookup table). 
 * 
 * void QEvalAVX2 :: generateStraights()
 * 		Generates the lookup table
 * 
 * uint8_t QEvalAVX2 :: findBestStraight(uint64_t cards) const
 * 		Returns the height of the highest card in the best possible straight
 * 		(A return value of zero means no straight can be found)
 * 
 ***************************************************************************************************/
 
uint32_t QEvalAVX2::score(uint64_t key) const { 
	
	//Constants
	static const int spades_offset   = 39;
	static const int clubs_offset    = 26;
	static const int diamonds_offset = 13;

	static const uint64_t mask13 = 0x0000000000001fff;
	static const uint64_t left1 = 0x8000000000000000;

	//Frequency offsets
	static const int info_offset = 26;
	static const int master_offset = 13;
	
	//compute suits
	uint64_t hearts = key & mask13;
	uint64_t diamonds = (key >> diamonds_offset) & mask13;
	uint64_t clubs = (key >> clubs_offset) & mask13;
	uint64_t spades =(key>> spades_offset) & mask13;
	
	//compute frequency masks
	uint64_t freq1 = hearts;
	uint64_t freq2 = freq1 & diamonds;
	freq1 |= diamonds;
	uint64_t freq3 = (freq2 & clubs);
	freq2 |= (freq1 & clubs);
	freq1 |= clubs;
	uint64_t freq4 = (freq3 & spades);
	freq3 |= (freq2 & spades);
	freq2 |= (freq1 & spades);
	freq1 |= spades;
	
	uint32_t straight_score = _straights[freq1];
	uint64_t score;
	
	int  count = popc(freq2);

	if(freq4) { //A card of frequency 4 detected, extremely unlikely
		freq1 -= freq4; //Remove the card of frequency four from the cards of frequency 1
		score = (7 << info_offset) | (freq4 << master_offset) | (left1 >> bsr(freq1)); //Compute the score
	} 
	else if(freq3) { //A card of frequency 3 detected, very unlikely
		if(!(count == 1)){ //Full house, very unlikely
			count = popc(freq3);
			if(count == 2) { //Two triples: unlikely
				freq3 = blsr(freq3); //Remove the lowest triple
				freq2-=freq3;
				score = (6 << info_offset) | (freq3 << master_offset) | freq2; //Compute the score

			}
			else {//Only one triple, more likely
				freq2-=freq3;
				score = (6 << info_offset) | (freq3 << master_offset) | (left1 >> bsr(freq2)); //Compute the score
			}
		}
		else { //3 of same kind: 3+1+1+1+1
			freq1 -=freq3; //Remove the card of frequency 3 from the cards of frequency 1
			freq1 = blsr(freq1); //Remove the two lowest cards of frequency 1
			freq1 = blsr(freq1);
			score = (3 << info_offset) | (freq3 << master_offset) | freq1; //Compute the score
		}
	}
	else if(count == 3) {
		freq2 = blsr(freq2);
		freq1-=freq2;
		freq1=blsr(freq1); //Remove the lowest two freq 1 cards
		score= (2 << info_offset) | (freq2 << master_offset) | freq1; //compute the score
	}
	else {
		//We are now in the situation of 2+1+1+1+1+1 or 2+2+1+1+1 or 1+1+1+1+1+1+1,
		//which is the most likely scenario. 
		freq1-=freq2; //Remove the pairs from the freq 1 cards
		freq1=blsr(freq1); //In the next two instructions, remove the lowest two freq 1 cards
		freq1=blsr(freq1);
		score= (count << info_offset) | (freq2 << master_offset) | freq1; //compute the score
	}


	if(score < 0x10000000 && straight_score){
		//printf("Replacing with straight\n");
		score = (4 << info_offset) | straight_score;
	}
	
	int suit = ((key>>52) & 0x7) >= 5 ? 1 : 0;
	suit = ((key>>55) & 0x7) >= 5 ? 2 : suit;
	suit = ((key>>58) & 0x7) >= 5 ? 3 : suit;
	suit = ((key>>61) & 0x7) >= 5 ? 4 : suit;

	if(suit) { //if suit == 0 there is no flush, else suit is 1 for hearts, 2 for diamonds, 3 for clubs, 4 for spades


		uint32_t flush_score=0;
		uint64_t suit_cards=0;
		//cout << (key >> 52) << endl;
		switch(suit) {	

			case 1:
				suit_cards = hearts;
				break;
			case 2:
				suit_cards = diamonds;
				break;
			case 3:
				suit_cards = clubs;
				break;
			case 4:
				suit_cards = spades;
				break;
		}
		if(flush_score = _straights[suit_cards]) //check for a straight flush
			return (8 << info_offset) | flush_score; //striaght flush found, return the score
		else { //Remove the lowest one or two cards of the suit, if needed
			count = popc(suit_cards);
			if(count == 6)
				suit_cards = blsr(suit_cards);
			else if(count == 7) {
				suit_cards = blsr(suit_cards);
				suit_cards = blsr(suit_cards);
			}
			flush_score = (5 << info_offset) | suit_cards; //compute the flush score
		}
		return score>flush_score? score : flush_score; 
	}
	else 
		return score;
}


void QEvalAVX2 :: generateStraights() {
	_straights = new uint8_t [8192];
	for(uint64_t i = 0; i!= 8192;i++) {
			_straights[i] = findBestStraight(i);
	}
}

//Function which finds the best straight in a set of cards
uint8_t QEvalAVX2 :: findBestStraight(uint64_t cards) const {
		static const uint64_t baby_straight_mask = 0x100f;
		static const uint64_t left1 = 0x8000000000000000;
		static const uint64_t mask = 0xf800000000000000;
		uint8_t best_straight = 0;
		
		if((cards & baby_straight_mask) == baby_straight_mask) {
			best_straight = 4;
		}
		
		int tries = popc(cards)-4; //Number of tries 
		int leadcount = 0; //The number of leading zeros before the best cards
		
		uint64_t smask = 0x1f00; // = 11111000000000
		
		leadcount = bsr(cards); //Set leadcount to number of zeros before the best card
		if(tries < 0)
			return 0;
		for(int i=0; i!= tries; i++) { //Here we try to find the best possible straight
			smask = mask >> leadcount;
			if((cards & smask) == smask) { //If found a straight, done and store the top value
				best_straight = 64-leadcount;
				break;
			}
			cards -= (left1 >> leadcount); //Else discard best card in the suit
			leadcount = bsr(cards);		   //Update leadcount
		}
		return best_straight;
}


/*******************************************************************************************************************
 * Batch scoring functions
 * 
 * void QEvalAVX2 :: batch16(uint64_t* keys, uint32_t* scores) const
 * 		This is a version of the single hand scoring function without any branching statments,
 * 		and which only uses bit manipulation - but in a vectorized fashion (16 hands are treated
 * 		at once for most of the code). This uses the vectorclass library for easy access to 
 * 		the AVX2 instructions. The lack of branching statements means more work, but the vectorization
 *		means that we are in the end more efficient: this function is between 2.5 and 3 times faster 
 *  	than the single hand scoring function. 
 * 
 * Vec16us QEvalAVX2 :: findStraight(Vec16us v) const 
 * 		Takes a 256bit vector of 16 16bit integers representing a set of at 
 * 		most 7 cards and returns a 256bit vector of 16 16bit integers containing
 * 		the height of the highest card in the best possible straight
 * 
 * void QEvalAVX2 :: score(uint64_t* keys, uint32_t* scores, uint64_t size) const 
 * 		Takes a pointer to an array of 64bit keys, and stores the 
 * 		corresponding scores in an array of 32bit integers
 * 
 ********************************************************************************************************************/

inline void QEvalAVX2::batch16(uint64_t* keys, uint32_t* scores) const
{
	//Read keys
	Vec4uq key1;
	Vec4uq key2;
	Vec4uq key3;
	Vec4uq key4;
	
	key1.load(keys);
	key2.load(keys+4);
	key3.load(keys+8);
	key4.load(keys+12);

	//Unpack suits and store them as vectors of 16 16-bits unsigned integers
	Vec4uq mask13(0x1fff);
	
	
	Vec16us hearts = (__m256i) ((key1 & mask13) << 48) ;
	Vec16us diamonds = (__m256i) (((key1 >> 13)& mask13) << 48);
	Vec16us clubs = (__m256i) (((key1 >> 26) & mask13) << 48);
	Vec16us spades = (__m256i) (((key1 >> 39) & mask13) << 48);
	
	Vec16us counts = (__m256i) ((key1>>52) << 48);
	
	hearts |= (__m256i) ((key3 & mask13) << 32);
	diamonds |= (__m256i) (((key3 >> 13)& mask13) << 32);
	clubs |= (__m256i) (((key3 >> 26) & mask13) << 32);
	spades |= (__m256i) (((key3 >> 39) & mask13) << 32);
	
	counts |= (__m256i) ((key3>>52) << 32);
	
	hearts |= (__m256i) ((key2 & mask13) << 16);
	diamonds |= (__m256i) (((key2 >> 13)& mask13) << 16);
	clubs |= (__m256i) (((key2 >> 26) & mask13) << 16);
	spades |= (__m256i) (((key2 >> 39) & mask13) << 16);
	
	counts |= (__m256i) ((key2>>52) << 16);

	hearts |= (__m256i) ((key4 & mask13));
	diamonds |= (__m256i) (((key4 >> 13)& mask13));
	clubs |= (__m256i) (((key4 >> 26) & mask13));
	spades |= (__m256i) (((key4 >> 39) & mask13));
	
	counts |= (__m256i) (key4>>52);

	//Compute frequencies
	Vec16us freq1 = hearts;
	Vec16us freq2 = freq1 & diamonds;
	freq1 |= diamonds;
	Vec16us freq3 = (freq2 & clubs);
	freq2 |= (freq1 & clubs);
	freq1 |= clubs;
	Vec16us freq4 = (freq3 & spades);
	freq3 |= (freq2 & spades);
	freq2 |= (freq1 & spades);
	freq1 |= spades;
	
	//Computations
	Vec16us straight = findStraight(freq1);
	freq1 -= freq4;
	Vec16us temp1 = rlsbc(rlsbc(freq1));
	
	freq3 = rlsbc(freq3);
	freq2 -= freq3;
	freq1 -= freq3;
	Vec16us temp2 = rlsbc(freq2);
	
	Vec16us extr1 = rlsb(freq2);
	Vec16us extr2 = rlsb(extr1);
	
	freq2 -= (Vec16us) (__m256i) andnot(lsb(freq2),extr2 == 0); //Remove least set bit if at least 3 bits
	Vec16us one(1);
	Vec16us temp = Vec16us(0) + (Vec16us) (__m256i) andnot(one, freq2 == 0) 
				+ (Vec16us) (__m256i)  andnot(one, extr1 == 0); //Compute the number of bits in freq2
	
	freq1 -= freq2;
	freq1 = rlsb(freq1);
	freq1 = rlsbc(freq1);
	
	Vec16us mask3(0x7);
	
	Vec16us current_count = counts & mask3;
	Vec16us count = (__m256i) (current_count >= 5) & current_count;
	Vec16us flush = (__m256i) (current_count >=5) & hearts;
	
	current_count = (counts >>3) & mask3;
	count |= (__m256i)  (current_count >= 5) & current_count;
	flush |= (__m256i) (current_count >=5) & diamonds;

	current_count = (counts >>6) & mask3;
	count |= (__m256i)  (current_count >= 5) & current_count;
	flush |= (__m256i) (current_count >=5) & clubs;
	
	current_count = (counts >>9) & mask3;
	count |= (__m256i)  (current_count >= 5) & current_count;
	flush |=  (__m256i)  (current_count >=5) & spades;
	
	Vec16us straight_flush = findStraight(flush);
	
	flush -= (Vec16us) ((count >= 6) & lsb(flush));
	flush -= (Vec16us) ((count == 7) & lsb(flush));

	//Switch to 8 keys per vector and compute the score
	//Score1 first
	Vec8ui mask16(0x0000ffff);
	Vec8ui freq4_red =(__m256i) ((Vec8ui) (freq4) & mask16);
	Vec8ui freq3_red =(__m256i) ((Vec8ui) (freq3) & mask16);
	Vec8ui freq2_red = (__m256i) ((Vec8ui) (freq2) & mask16);
	Vec8ui freq1_red = (__m256i) ((Vec8ui) (freq1) & mask16);
	Vec8ui temp_red = (__m256i) ((Vec8ui) (temp) & mask16);
	Vec8ui temp1_red = (__m256i) ((Vec8ui) (temp1) & mask16);
	Vec8ui temp2_red = (__m256i) ((Vec8ui) (temp2) & mask16);
	Vec8ui straight_red = (__m256i)(straight & mask16);
	Vec8ui flush_red = (__m256i) ((Vec8ui) (flush) & mask16);
	Vec8ui straight_flush_red = (__m256i) ((Vec8ui) (straight_flush) & mask16);
	
	Vec8ui score4K =(__m256i) andnot((Vec8ui) (( 7 << 26) | (freq4_red << 13) | temp1_red), freq4_red == 0);
	Vec8ui scoreFH =(__m256i) andnot((Vec8ui) (( 6 << 26) | (freq3_red << 13) 
					| temp2_red), (freq3_red == 0) | (freq2_red ==0)); 	
	Vec8ui score3K =(__m256i) andnot((Vec8ui) (( 3 << 26) | (freq3_red << 13) | freq1_red), freq3_red == 0);
	Vec8ui score12 = (__m256i)(((temp_red & Vec8ui(0xf)) << 26) | (freq2_red << 13) | freq1_red);
	Vec8ui score_straight =(__m256i) andnot((4<<26) | straight_red, straight_red == 0);
	Vec8ui score_flush = (__m256i) andnot((5<<26) | flush_red, flush_red == 0);
	Vec8ui score_straight_flush = (__m256i) andnot((8<<26) | straight_flush_red, straight_flush_red == 0);
	
	Vec8ui score1 = max(max(max(score4K,scoreFH), max(score3K,score12)),
					max(max(score_straight, score_flush),score_straight_flush));
	
	//Now Score2
	freq4_red = ((Vec8ui) freq4) >> 16;
	freq3_red = ((Vec8ui) freq3) >> 16;
	freq2_red =  ((Vec8ui) freq2) >> 16;
	freq1_red = ((Vec8ui) freq1) >> 16;
	temp_red = ((Vec8ui) temp) >> 16;
	temp1_red = ((Vec8ui) temp1) >> 16;
	temp2_red = ((Vec8ui) temp2) >> 16;
	straight_red = ((Vec8ui) straight) >> 16;
	flush_red = ((Vec8ui) flush) >> 16;
	straight_flush_red = ((Vec8ui) straight_flush) >> 16;
	
	score4K =(__m256i) andnot((Vec8ui) (( 7 << 26) | (freq4_red << 13) | temp1_red), freq4_red == 0);
	scoreFH =(__m256i) andnot((Vec8ui) (( 6 << 26) | (freq3_red << 13)
					| temp2_red), (freq3_red == 0) | (freq2_red ==0)); 	
	score3K =(__m256i) andnot((Vec8ui) (( 3 << 26) | (freq3_red << 13) | freq1_red), freq3_red == 0);
	score12 = (__m256i)(((temp_red & Vec8ui(0xf)) << 26) | (freq2_red << 13) | freq1_red);
	score_straight =(__m256i) andnot((4<<26) | straight_red, straight_red == 0);
	score_flush = (__m256i) andnot((5<<26) | flush_red, flush_red == 0);
	score_straight_flush = (__m256i) andnot((8<<26) | straight_flush_red, straight_flush_red == 0);
	
	Vec8ui score2 = max(max(max(score4K,scoreFH), max(score3K,score12)),
				max(max(score_straight, score_flush),score_straight_flush));

	//Done computing both scores, time to write back
	score1 = _mm256_permutevar8x32_epi32(score1, _index);
	score2 = _mm256_permutevar8x32_epi32(score2, _index);
	score2.store(scores);
	score1.store(scores+8);
}


inline Vec16us QEvalAVX2::findStraight(Vec16us v) const {
	Vec16us mask(0x100f);
	Vec16us best_straight = max(  (Vec16us) (((mask & v) == mask) & Vec16us(4)),0);
	Vec16us counter(5);
	Vec16us one(1);
	mask = 0x1f;
	
	for(int i=0; i!=9; i++) {
		best_straight = max((Vec16us) (((mask & v) == mask) & counter) ,best_straight);
		counter += one;
		mask  = mask << 1;
	}
	return best_straight;
}

inline void QEvalAVX2::score(uint64_t* keys, uint32_t* scores, uint64_t size) const {
	uint64_t tot = size/16;
	for(uint64_t i=0; i!=tot; i++) {
		batch16(keys, scores);
		scores += 16;
		keys += 16;
	}
	uint64_t rem = size % 16;
	for(int i=0; i!=rem; i++) {
		scores[i] = score(keys[i]);
	}
}


/***********************************************************************************************
 * Constructor
 * 
 * Intializes the table of straights and checks whether 
 * the cpu supports the BMI1 and AVX2 instruction sets
 * 
 ***********************************************************************************************/
 

QEvalAVX2::QEvalAVX2():_straights(nullptr) {
	if(!__builtin_cpu_supports("avx2"))
		throw std::runtime_error("No AVX2 support detected.");
	
	if(!__builtin_cpu_supports("bmi"))
		throw std::runtime_error("No BMI1 support detected.");
	
	if(!__builtin_cpu_supports("popcnt"))
		throw std::runtime_error("No POPCNT support detected.");
		
	_index = Vec8ui(1,3,5,7,0,2,4,6);
	generateStraights();
}

/***********************************************************************************************
 * Destructor
 * 
 * Deletes the look-up table
 * 
 ***********************************************************************************************/
 
QEvalAVX2 :: ~QEvalAVX2() {
	delete [] _straights;
}
