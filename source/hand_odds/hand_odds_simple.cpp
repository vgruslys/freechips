#include "hand_odds_simple.h"

pair<float,float> HandOddsSimple :: calculateOdds(int street, Card* table_cards, Card* own_cards, Card* opp_cards) const
{
    Card full_table[5];
    for (int i = 0; i < street; ++i)
        full_table[i] = table_cards[i];

    if (street == 0) {
        int16_t 
    }
}

pair<float,float> HandOddsSimple :: calculateOddsNoTable(Card* own_cards, Card* opp_cards) const
{

}

int16_t HandOddsSimple :: cardToInt(Card card) const
{
    return getSuit(card) * 13 + getHeight(card) - 2;
}
