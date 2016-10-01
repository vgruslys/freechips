#ifndef __PLAYER_CARDS__
#define __PLAYER_CARDS__
#include "card.h"


/*PlayerCards - an obvious class*/

class PlayerCards {
	public:
		void addCard(Card);                         // Add a card to the player's hand
		void removeCard(Card);                      // Removes a specified card (trying to remove a card which was not added before will lead to undefined behaviour)
		void reset();                               // Removes all cards from the player's hand

	    uint64_t getKey() const;                    // Returns the 52bit key, where each card is represented by a single bit

		PlayerCards();
	private:
		uint64_t _key;
		static const uint64_t _card_map [52]; //This points to a 52-entry array which converts a Card to a a uint64_t 
							// of the form 0000100
};
#endif
