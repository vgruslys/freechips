#include "sets.h"

const HandValue ZEROTH_MASK = 0x00f00000;
const HandValue FIRST_MASK  = 0x000f0000;
const HandValue SECOND_MASK = 0x0000f000;
const HandValue THIRD_MASK  = 0x00000f00;
const HandValue FOURTH_MASK = 0x000000f0;
const HandValue FIFTH_MASK  = 0x0000000f;

int Set::_globally_initialised = 0;
HandValue Set::_best_straight[8192];
HandValue Set::_best_cards[8192];

int8_t max(int8_t a, int8_t b, int8_t c, int8_t d)
{
    return std::max(std::max(a, b), std::max(c, d));
}

Set :: Set(Card* community, Card card1, Card card2)
{
    if (!_globally_initialised)
        initialise();

    for (int i = 0; i < 4; ++i) {
        _suit_mask[i] = 0;
        _suit_count[i] = 0;
        _frequency_mask[i] = 0;
    }
    for (int i = 0; i < 13; ++i)
        _height_map[i] = 0;

    Height height;
    Suit suit;

    for (int i = 0; i < 5; ++i) {
        height = getHeight(community[i]) - 2;
        suit = getSuit(community[i]);
        addCard(suit, height);
    }

    height = getHeight(card1) - 2;
    suit = getSuit(card1);
    addCard(suit, height);

    height = getHeight(card2) - 2;
    suit = getSuit(card2);
    addCard(suit, height);

    calculate();
}

void Set :: addCard(Suit suit, Height height)
{
    _suit_mask[suit] |= ((Card) 1 << height);
    ++_suit_count[suit];
    ++_height_map[height];

    if (_height_map[height] <= 4)
        _frequency_mask[_height_map[height]-1] |= ((Card) 1 << height);
    else
        throw CardError("Cannot have more than four cards of the same height");
}

HandValue Set :: findRoyalFlush() const
{
    int8_t best_straight_flush = max(
            _best_straight[_suit_mask[0]],
            _best_straight[_suit_mask[1]],
            _best_straight[_suit_mask[2]],
            _best_straight[_suit_mask[3]]);

    if (best_straight_flush == 14)
        return H_ROYAL_FLUSH << 20;
    else
        return 0;
}

HandValue Set :: findStraightFlush() const
{
    HandValue best_straight_flush = max(
            _best_straight[_suit_mask[0]],
            _best_straight[_suit_mask[1]],
            _best_straight[_suit_mask[2]],
            _best_straight[_suit_mask[3]]);

    if (best_straight_flush != 0)
        return (H_STRAIGHT_FLUSH << 20) | (best_straight_flush << 16);
    else
        return 0;
}

HandValue Set :: findFourOfAKind() const
{
    HandValue fours = _best_cards[_frequency_mask[3]];

    if (fours) {
        HandValue best_four = (fours & FIRST_MASK) >> 16;
        HandValue kickers = _best_cards[_frequency_mask[0] & ~((HandValue) 1 << (best_four-2))];
        HandValue best_kicker = (kickers & FIRST_MASK) >> 16;

        return (H_FOUR_OF_A_KIND << 20) | (best_four << 16) | (best_kicker << 12);
    }
    else
        return 0;
}

HandValue Set :: findFullHouse() const
{
    HandValue threes = _best_cards[_frequency_mask[2]];

    if (threes) {
        HandValue best_three = (threes & FIRST_MASK) >> 16;
        HandValue twos = _best_cards[_frequency_mask[1] & ~((HandValue) 1 << (best_three-2))];
        if (twos) {
            HandValue best_two = (twos & FIRST_MASK) >> 16;
            return (H_FULL_HOUSE << 20) | (best_three << 16) | (best_two << 12);
        }
        else
            return 0;
    }
    else
        return 0;
}

HandValue Set :: findFlush() const
{
    int8_t suit = -1;

    for (int i = 0; i < 4; ++i)
        if (_suit_count[i] >= 5)
            suit = i;

    if (suit != -1) {
        HandValue strength = _best_cards[_suit_mask[suit]];
        return (H_FLUSH << 20) | strength;
    }
    else
        return 0;
}

HandValue Set :: findStraight() const
{
    HandValue best_straight = _best_straight[_frequency_mask[0]];

    if (best_straight)
        return (H_STRAIGHT << 20) | (best_straight << 16);
    else
        return 0;
}

HandValue Set :: findThreeOfAKind() const
{
    HandValue threes = _best_cards[_frequency_mask[2]];

    if (threes) {
        HandValue best_three = (threes & FIRST_MASK) >> 16;
        HandValue kickers = _best_cards[_frequency_mask[0] & ~((HandValue) 1 << (best_three-2))];
        HandValue best_kickers = (kickers & (FIRST_MASK | SECOND_MASK)) >> 12;
        return (H_THREE_OF_A_KIND << 20) | (best_three << 16) | (best_kickers << 8);
    }
    else
        return 0;
}

HandValue Set :: findTwoPair() const
{
    HandValue twos = _best_cards[_frequency_mask[1]];

    if (twos & ~FIRST_MASK) {
        HandValue hi_two = (twos & FIRST_MASK) >> 16;
        HandValue lo_two = (twos & SECOND_MASK) >> 12;
        HandValue kickers = _best_cards[_frequency_mask[0] & (~((HandValue) 1 << (hi_two-2)) & (~((HandValue) 1 << (lo_two-2))))];
        HandValue best_kicker = (kickers & FIRST_MASK) >> 16;
        return (H_TWO_PAIR << 20) | (hi_two << 16) | (lo_two << 12) | (best_kicker << 8);
    }
    else
        return 0;
}

HandValue Set :: findPair() const
{
    HandValue twos = _best_cards[_frequency_mask[1]];

    if (twos) {
        HandValue best_two = (twos & FIRST_MASK) >> 16;
        HandValue kickers = _best_cards[_frequency_mask[0] & ~((HandValue) 1 << (best_two-2))];
        HandValue best_kickers = (kickers & (FIRST_MASK | SECOND_MASK | THIRD_MASK)) >> 8;
        return (H_PAIR << 20) | (best_two << 16) | (best_kickers << 4);
    }
    else
        return 0;
}

HandValue Set :: findHighCard() const
{
    HandValue kickers = _best_cards[_frequency_mask[0]];
    return (H_HIGH_CARD << 20) | kickers;
}

void Set :: initialise()
{
    for (int16_t i = 0; i < 8192; ++i)
        _best_straight[i] = 0;

    int16_t baby_straight = 0x100f;
    for (int16_t i = 0; i < 256; ++i)
        _best_straight[baby_straight | (i << 4)] = 5;

    int16_t straight = 0x001f;
    for (int8_t s = 0; s < 9; ++s)
        for (int16_t i = 0; i < 8192; ++i) // Can be done faster
            _best_straight[(straight << s) | i] = s + 6;

    _best_cards[0] = 0;
    for (HandValue top = 0; top < 13; ++top)
        for (int16_t i = 0; i < (1 << top); ++i)
            _best_cards[(1 << top) | i] = ((top + 2) << 16) | (_best_cards[i] >> 4);

    _globally_initialised = 1;
}

void Set :: calculate()
{
    _value = findRoyalFlush();

    if (!_value)
        _value = findStraightFlush();

    if (!_value)
        _value = findFourOfAKind();

    if (!_value)
        _value = findFullHouse();

    if (!_value)
        _value = findFlush();

    if (!_value)
        _value = findStraight();

    if (!_value)
        _value = findThreeOfAKind();

    if (!_value)
        _value = findTwoPair();

    if (!_value)
        _value = findPair();

    if (!_value)
        _value = findHighCard();
}

HandValue Set :: getValue() const
{
    return _value;
}

HandValue getHandValuePosition(HandValue value, int pos)
{
    switch (pos) {
        
        case 0 :
            return (value & ZEROTH_MASK) >> 20;
            break;
        case 1 :
            return (value & FIRST_MASK) >> 16;
            break;
        case 2 :
            return (value & SECOND_MASK) >> 12;
            break;
        case 3 :
            return (value & THIRD_MASK) >> 8;
            break;
        case 4 :
            return (value & FOURTH_MASK) >> 4;
            break;
        case 5 :
            return value & FIFTH_MASK;
            break;
        default :
            throw CardError("Accessing invalid position of HandValue");
    
    }
}

HandValue getHandValueType(HandValue value)
{
    return getHandValuePosition(value, 0);
}
