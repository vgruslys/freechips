#ifndef __PLAYER_CARDS__
#define __PLAYER_CARDS__
#include "card.h"


/*PlayerCards - an obvious class*/

class PlayerCards {
	public:
		void addCard(Card);
		void removeCard();
		
		uint64_t getKey() const;
};
#endif