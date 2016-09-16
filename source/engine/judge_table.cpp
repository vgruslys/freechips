#include "judge_table.h"
#include "community.h"
#include "player_cards.h"
#include "key_numbers.h"
#include <iostream>
using namespace std;
int JudgeTable::verdict(const Community& com, const PlayerCards& pc) const {
	
	/*static internal variables */
	static int _flush_score =0; //stores the flush score
	static  int _unsuited_score =0; //stores the unsuited score
	
	//keys
	static uint64_t _key= 0; //the normal 64bit community key
	static uint64_t _coded_key = 0; //the coded community key 
	static uint64_t _suit_key = 0; //the small suit key extrated from the coded community key
	static uint64_t _unsuited_key = 0; //the unsuited key extracted from the coded community key 
	static uint64_t _suit_shift = 0; // the bit shift deduced from the _suit_key
	static uint64_t _player_key = 0; // the normal 64bit player key
	static uint64_t _player_unsuited_key = 0; // the player unsuited key (should be a small int representing the height of the two cards that he has)
	
	//masks
	static uint64_t _mask13 = 0x0000000000001fff;
	static uint64_t _mask32 = 0x00000000ffffffff;
	
	/*Load community keys */
	_key = com.getKey();
	_coded_key = com.getCodedKey();
	
	/*extract the two coded keys */
	_unsuited_key = _coded_key & _mask32;
	_suit_key = _coded_key >> 32;
	
	/*Load player keys*/
	_player_key = pc.getKey();
	_player_unsuited_key = pc.getCodedKey();
	
	/*Find the suit which could potentially give a flush, & the corresponding bit shift*/
	_suit_shift = _suit_map[_suit_key];

	/*Compute flush score */
	_flush_score = _flush_table[((_key >> _suit_shift) & _mask13) | ((_player_key >> _suit_shift) & _mask13)];
	/*Compute the unsuited score*/
	_unsuited_score = _unsuited_table[_unsuited_key][_player_unsuited_key];
	
	return _flush_score > _unsuited_score? _flush_score : _unsuited_score; // returns max of flush and unsuited score
}

JudgeTable :: JudgeTable(): _suit_map(new int [MAX_SUIT_NUMBERS_5SUM + 1]),
						  _flush_table(new int [8192]),
						  _unsuited_table(new int* [MAX_HEIGHT_NUMBERS_5SUM + 1])
						  
{
	for(int i=0; i!=MAX_SUIT_NUMBERS_5SUM + 1; i++) {
		_suit_map[i] = 0;
	}
	for(int i=0; i!= 8192; i++) {
		_flush_table[i] = 0;
	}
	
	for(int i=0; i!= MAX_HEIGHT_NUMBERS_5SUM + 1; i++) {
		_unsuited_table[i] = NULL;
	}
	generateFiveHighestCards();
    generateHighestStraight();
	generateTables();
	
}

JudgeTable :: ~JudgeTable () {
	delete [] _suit_map;
	delete [] _flush_table;
	for(int i=0; i!= MAX_HEIGHT_NUMBERS_5SUM + 1; i++)
		delete [] _unsuited_table[i];
	delete [] _unsuited_table;
	delete[] _five_highest_cards;
    delete[] _highest_straight;
	
}
void JudgeTable :: generateTables() {
	Community com;
	PlayerCards pc;
	
	//First we construct the flush table; 13 choose 7 combinations to check
	recFlushTable(com,0,0);
	
	//Now we construct the unsuited table
	com.reset();
	
	int freq_table[13];
	for(int i=0; i!=13; i++)
		freq_table[i] = 0;
	recUnsuitedTable(com,0,0,freq_table);
	
	com.reset();
	//Let us not forget the suit map
	recSuitMap(com, 0,0);
}

void JudgeTable :: recFlushTable(Community& com, int start, int depth) {
	_flush_table[com.getKey()] = flushScore(com.getKey());
	if(depth < 7) {
		for(int i=start; i!= 13; i++) {
			com.addCard(i);
			this->recFlushTable(com, i +1, depth + 1);
			com.removeCard(i);
		}
	}
}

void JudgeTable :: recUnsuitedTable(Community& com, int start, int depth, int* freq_table) {
	if(depth == 5) {
		
		if(_unsuited_table[com.getCodedKey()&0x00000000ffffffff] == NULL) {
		   _unsuited_table[com.getCodedKey()&0x00000000ffffffff] = new int [MAX_HAND_HEIGHT_NUMBERS_2SUM+1];
		}
		
		PlayerCards pc;
		this->recUnsuitedTablePlayer(pc,com,0, 0, freq_table);
		
	}
	else {
		for(int i=start; i!= 13; i++) {
			if(freq_table[i] < 4) {
				com.addCard(i+13*freq_table[i]);
				freq_table[i]++;
				this->recUnsuitedTable(com, i, depth+1, freq_table);
				freq_table[i]--;
				com.removeCard( i+13*freq_table[i] );
			}
		}
	}
}

void JudgeTable :: recUnsuitedTablePlayer(PlayerCards& pc, Community& com, int start, int depth, int* freq_table) {
	if(depth == 2) {
 		_unsuited_table[(com.getCodedKey() & 0x00000000ffffffff)][pc.getCodedKey()]= unsuitedScore(com.getKey() | pc.getKey());
	}
	else {
		for(int i=start; i!=13; i++) {
			if(freq_table[i]<4) {
 				pc.addCard(i+13*freq_table[i]);
				freq_table[i]++;
				this->recUnsuitedTablePlayer(pc,com,i,depth+1,freq_table);
				freq_table[i]--;
				pc.removeCard(i+13*freq_table[i]);
			}
		}
	}
}

void JudgeTable :: recSuitMap(Community& com, int start, int depth) {
	
	if(depth == 5) {
		if(__builtin_popcountll(com.getKey() & 0x0000000000001fff) >=3)
			_suit_map[com.getCodedKey()>>32] = 0;
		else if (__builtin_popcountll((com.getKey()>>13) & 0x0000000000001fff) >=3)
			_suit_map[com.getCodedKey()>>32] = 13;
		else if (__builtin_popcountll((com.getKey()>>26) & 0x0000000000001fff) >=3)
			_suit_map[com.getCodedKey()>>32] = 26;
		else if (__builtin_popcountll((com.getKey()>>39) & 0x0000000000001fff) >=3)
			_suit_map[com.getCodedKey()>>32] = 39;
		else 
			_suit_map[com.getCodedKey()>>32] = 52;
	}
	else {
		for(int i=start; i!=4; i++) {
			com.addCard(i*13);
			recSuitMap(com, i, depth+1);
			com.removeCard(i*13);
		}
			
	}

}



/*Vytautas functions */
void JudgeTable :: generateFiveHighestCards()
{
    _five_highest_cards = new uint64_t[1 << 13];

    _five_highest_cards[0] = 0;
    for (int bit = 0; bit < 13; bit = ++bit)
       for (uint64_t suffix = 0; suffix < (1 << bit); ++suffix)
          _five_highest_cards[(1 << bit) | suffix] = ((bit + 2) << 16) | (_five_highest_cards[suffix] >> 4);
}

void JudgeTable :: generateHighestStraight()
{
    _highest_straight = new uint8_t[1 << 13];

    for (uint64_t key = 0; key < (1 << 13); ++key)
        if (__builtin_popcountll(key) >= 5)
            _highest_straight[key] = 0x10;

    uint64_t baby_straight = 0x100f;
    for (uint64_t dummy = 0; dummy < (1 << 8); ++dummy)
        _highest_straight[baby_straight | (dummy << 4)] = 0x15;

    uint64_t straight = 0x1f;
    for (int offset = 0; offset <= 8; ++offset)
        for (uint64_t prefix = 0; prefix < (1 << (8 - offset)); ++prefix)
            for (uint64_t suffix = 0; suffix < (1 << offset); ++suffix)
                _highest_straight[(straight << offset) | (prefix << (offset + 5)) | suffix] = 0x10 | (offset + 6);
}

int JudgeTable :: flushScore(uint64_t key) const
{
    uint8_t straight = _highest_straight[key];

    if (straight) {                     // This implies that there are at least five suited cards.

        if (straight == 0x1e)           // This implies royal flush.
            return 0x900000;
        
        else if (straight & 0xf)        // This implies straight flush.
            return 0x800000 | ((straight & 0xf) << 16);
        
        else                            // This implies flush.
            return 0x500000 | _five_highest_cards[key];

    }

    else
        return 0;
		
}

int JudgeTable :: unsuitedScore(uint64_t key) const
{
    uint64_t frequency_masks[4] = {0};  // These masks show which cards appear at least i+1 times, 0 <= i <= 3.

    int spades_offset   = 0;
    int clubs_offset    = 13;
    int diamonds_offset = 26;
    int hearts_offset   = 39;

    uint64_t spades   = (key & (0x1fffLL << spades_offset))   >> spades_offset;
    uint64_t clubs    = (key & (0x1fffLL < clubs_offset))     >> clubs_offset;
    uint64_t diamonds = (key & (0x1fffLL << diamonds_offset)) >> diamonds_offset;
    uint64_t hearts   = (key & (0x1fffLL << hearts_offset))   >> hearts_offset;

    frequency_masks[0] = spades;

    frequency_masks[1] = frequency_masks[0] & clubs;
    frequency_masks[0] |= clubs;

    frequency_masks[2] = frequency_masks[1] & diamonds;
    frequency_masks[1] |= frequency_masks[0] & diamonds;
    frequency_masks[0] |= diamonds;

    frequency_masks[3] = frequency_masks[2] & hearts;
    frequency_masks[2] |= (frequency_masks[1] & hearts);
    frequency_masks[1] |= (frequency_masks[0] & hearts);
    frequency_masks[0] |= hearts;

    
    if (frequency_masks[3]) {                               // This implies four-of-a-kind.
        uint64_t fours  = (_five_highest_cards[frequency_masks[3]] & 0xf0000) >> 16;
        uint64_t kicker = (_five_highest_cards[frequency_masks[0] & ~(1LL << fours)] & 0xf0000) >> 4;
        return 0x700000 | (fours << 16) | kicker;
    }

    else if (frequency_masks[2]) {                          // This implies full-house or three-of-a-kind.
        uint64_t threes = (_five_highest_cards[frequency_masks[2]] & 0xf0000) >> 16;
        uint64_t twos   = (_five_highest_cards[frequency_masks[1] & ~(1LL << threes)] & 0xf0000) >> 4;

        if (twos)                                           // This implies full-house
            return 0x600000 | (threes << 16) | twos;
        
        else {                                              // This implies three-of-a-kind.
            uint64_t kickers = (_five_highest_cards[frequency_masks[0] & ~(1LL << threes)] & 0xff000) >> 4;
            return 0x300000 | (threes << 16) | kickers;
        }
    }

    else if (frequency_masks[1]) {                          // This implies two-pair or one-pair.
        uint64_t twos_hi = (_five_highest_cards[frequency_masks[1]] & 0xf0000) >> 16;
        uint64_t twos_lo = (_five_highest_cards[frequency_masks[1]] & 0x0f000) >> 12;

        if (twos_lo) {                                     // This implies two-pair.
            uint64_t kicker = (_five_highest_cards[frequency_masks[0] & ~(1LL << twos_hi) & ~(1LL << twos_lo)] & 0xf0000) >> 8;
            return 0x200000 | (twos_hi << 16) | (twos_lo << 16) | kicker;
        }

        else {                                              // This implies one-pair.
            uint64_t kickers = (_five_highest_cards[frequency_masks[0] & ~(1LL << twos_hi)] & 0xfff00) >> 4;
            return 0x100000 | (twos_hi << 16) | kickers;
        }
    }

    else                                                    // This implies no hand (high-card).
        return _five_highest_cards[frequency_masks[0]];
}