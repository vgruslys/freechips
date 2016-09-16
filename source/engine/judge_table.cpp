#include "judge_table.h"
#include "community.h"
#include "player_cards.h"
#include "key_numbers.h"

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
	generateTables();
	
}

JudgeTable :: ~JudgeTable () {
	delete [] _suit_map;
	delete [] _flush_table;
	for(int i=0; i!= MAX_HEIGHT_NUMBERS_5SUM + 1; i++)
		delete [] _unsuited_table[i];
	delete [] _unsuited_table;
	
}
void JudgeTable :: generateTables() {
	Community com;
	PlayerCards pc;
	
	//First we construct the flush table; 13 choose 7 combinations to check
	recFlushTable(com,0,0);
	
	//Now we construct the unsuited table
	com.reset();
	
	int freq_table[13] {0};
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
		int* ptable = _unsuited_table[com.getCodedKey() & 0x00000000ffffffff];
		if(ptable == NULL) {
			ptable = new int [MAX_HAND_HEIGHT_NUMBERS_2SUM+1];
		}
		
		ptable[pc.getCodedKey()] = unsuitedScore(com.getKey() | pc.getKey());
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
			com.addCard(1<< (i*13));
			recSuitMap(com, i, depth+1);
			com.removeCard(1<< (i*13));
		}
			
	}

}
int JudgeTable :: flushScore(uint64_t) const {
	return 1;
}

int JudgeTable :: unsuitedScore(uint64_t) const {
	return 1;
}