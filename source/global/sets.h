#ifndef __SETS__
#define __SETS__


#include "card.h"

#include <cstdint>
#include <algorithm>

typedef std::int32_t HandValue;
typedef std::int16_t HeightMask;

const HandValue H_ROYAL_FLUSH     = 9;
const HandValue H_STRAIGHT_FLUSH  = 8;
const HandValue H_FOUR_OF_A_KIND  = 7;
const HandValue H_FULL_HOUSE      = 6;
const HandValue H_FLUSH           = 5;
const HandValue H_STRAIGHT        = 4;
const HandValue H_THREE_OF_A_KIND = 3;
const HandValue H_TWO_PAIR        = 2;
const HandValue H_PAIR            = 1;
const HandValue H_HIGH_CARD       = 0;

/*Set - a class which computes the best card combination out of a pair of cards and  5 street cards */

/*Usage: Set(ptr_to_street, card1, card2) set;
 * 
 * To compare two set of cards do
 * Set(ptr_to_street, card1, card2) set1;
 * Set(ptr_to_street, card3, card4) set2;
 * and then:
 * 
 * set1.getValue() < set2.getValue();
 * 
 * OR
 * 
 * set1 < set2;
*/
class Set {
    
    public:
        
        Set(Card*, Card, Card);
        
        HandValue getValue() const;
        bool operator<(const Set& );
        bool operator==(const Set& );
        bool operator>(const Set& );
    private:

        HandValue findRoyalFlush() const;
        HandValue findStraightFlush() const;
        HandValue findFourOfAKind() const;
        HandValue findFullHouse() const;
        HandValue findFlush() const;
        HandValue findStraight() const;
        HandValue findThreeOfAKind() const;
        HandValue findTwoPair() const;
        HandValue findPair() const;
        HandValue findHighCard() const;
        
        void calculate();
        void initialise();
        
        void addCard(Suit, Height);


        HeightMask _suit_mask[4];
        std::int8_t _suit_count[4];

        HeightMask _frequency_mask[4];
        std::int16_t _height_map[13];

        HandValue _value;

        static HandValue _best_cards[8192];
        static HandValue _best_straight[8192];
        static int _globally_initialised;

};

HandValue getHandValuePosition(HandValue, int);
HandValue getHandValueType(HandValue);

#endif
