#include "judge_20M.h"
#include "community.h"
#include "player_cards.h"
#include "key_numbers.h"
#include <iostream>

using namespace std;

int Judge20M::verdict(const Community& com, const PlayerCards& pc) const
{
    uint64_t key = com.getKey() | pc.getKey();

    int _flush_score = flushScore(key & 0x1fffLL) |
                       flushScore((key & (0x1fffLL << 13)) >> 13) |
                       flushScore((key & (0x1fffLL << 26)) >> 26) |
                       flushScore((key & (0x1fffLL << 39)) >> 39);

    int _unsuited_score = unsuitedScore(com.getKey() | pc.getKey());

	return _flush_score > _unsuited_score ? _flush_score : _unsuited_score; // returns max of flush and unsuited score
}

Judge20M :: Judge20M()
{
	generateFiveHighestCards();
    generateHighestStraight();
}

Judge20M :: ~Judge20M ()
{
	delete[] _five_highest_cards;
    delete[] _highest_straight;
}

void Judge20M :: generateFiveHighestCards()
{
    _five_highest_cards = new uint64_t[1 << 13];

    _five_highest_cards[0] = 0;
    for (int bit = 0; bit < 13; bit = ++bit)
       for (uint64_t suffix = 0; suffix < (1 << bit); ++suffix)
          _five_highest_cards[(1 << bit) | suffix] = ((bit + 2) << 16) | (_five_highest_cards[suffix] >> 4);
}

void Judge20M :: generateHighestStraight()
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

int Judge20M :: flushScore(uint64_t key) const
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

void printBytes(uint64_t mask)
{
    for (int i = 63; i >= 0; --i) {
        if (mask & (1LL << i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

int Judge20M :: unsuitedScore(uint64_t key) const
{
    uint64_t frequency_masks[4] = {0};  // These masks show which cards appear at least i+1 times, 0 <= i <= 3.

    int spades_offset   = 39;
    int clubs_offset    = 26;
    int diamonds_offset = 13;
    int hearts_offset   = 0;

    uint64_t spades   = (key & (0x1fffLL << spades_offset))   >> spades_offset;
    uint64_t clubs    = (key & (0x1fffLL << clubs_offset))    >> clubs_offset;
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
/*
    printf("singles = "); printBytes(frequency_masks[0]); 
    printf("doubles = "); printBytes(frequency_masks[1]); 
    printf("triples = "); printBytes(frequency_masks[2]); 
    printf("fours   = "); printBytes(frequency_masks[3]); 
*/
    if (frequency_masks[3]) {                               // This implies four-of-a-kind.
        uint64_t fours  = (_five_highest_cards[frequency_masks[3]] & 0xf0000) >> 16;
        uint64_t kicker = (_five_highest_cards[frequency_masks[0] & ~(1LL << (fours-2))] & 0xf0000) >> 4;
        return 0x700000 | (fours << 16) | kicker;
    }

    else if (frequency_masks[2]) {                          // This implies full-house or three-of-a-kind.
        uint64_t threes = (_five_highest_cards[frequency_masks[2]] & 0xf0000) >> 16;
        uint64_t twos   = (_five_highest_cards[frequency_masks[1] & ~(1LL << (threes-2))] & 0xf0000) >> 4;

        if (twos)                                           // This implies full-house
            return 0x600000 | (threes << 16) | twos;
        
        else {                                              // This implies three-of-a-kind.
            uint64_t kickers = (_five_highest_cards[frequency_masks[0] & ~(1LL << (threes-2))] & 0xff000) >> 4;
            return 0x300000 | (threes << 16) | kickers;
        }
    }

    else if (frequency_masks[1]) {                          // This implies two-pair or one-pair.
        uint64_t twos_hi = (_five_highest_cards[frequency_masks[1]] & 0xf0000) >> 16;
        uint64_t twos_lo = (_five_highest_cards[frequency_masks[1]] & 0x0f000) >> 12;

        if (twos_lo) {                                     // This implies two-pair.
            uint64_t kicker = (_five_highest_cards[frequency_masks[0] & ~(1LL << (twos_hi-2)) & ~(1LL << (twos_lo-2))] & 0xf0000) >> 8;
            return 0x200000 | (twos_hi << 16) | (twos_lo << 12) | kicker;
        }

        else {                                              // This implies one-pair.
            uint64_t kickers = (_five_highest_cards[frequency_masks[0] & ~(1LL << (twos_hi-2))] & 0xfff00) >> 4;
            return 0x100000 | (twos_hi << 16) | kickers;
        }
    }

    else                                                    // This implies no hand (high-card).
        return _five_highest_cards[frequency_masks[0]];
}
