#ifndef __YOUR_HONOUR__
#define __YOUR_HONOUR__
#include "community.h"
#include "player_cards.h"

class YourHonour : public Judge {
	public:
	int verdict(const Community&, const PlayerCards&) const; // the judge renders its verdict, which is an int 
														// representing the value of the best combination
	YourHonour();
private:
	int score(const Community&, const PlayerCards&) const; // when the judge has no look up table, he must use this to find the verdict
	int* _flush_table; //This table is used to look up a best flush combination; it is accessed as follows:
					  // _flush_table[13bit number] where the 13bit number in question represents a set of cards of a given suit
	int** _unsuited_table; //This table is used to find the best unsuited combination and is accessed as follows:
						//_unsuited_table[unsuited_key][unsuited_player_key] where the unsuited key is the community coded large key
						//and the unsuited player key is a small int representing the player hand
	int* _suit_map; //  maps a small suit key to a suit [bit position, i.e. 0, 13, 26, or 39 so we know by how much to shift to etc]
					//This is a small array used to find the suit which could give a flush
	void generateTables(); // generates the tables, called by the constructor
};
#endif