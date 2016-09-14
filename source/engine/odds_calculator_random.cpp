#include "odds_calculator_random.h"
#include "random_deck_plain.h"
#include "sets.h"
#include <cmath>
#include <utility>
#include <iostream>

using namespace std;
void OddsCalculatorRandom::setConfidence(float confidence) {
    _confidence = confidence;
    updateTrials();
} 

void OddsCalculatorRandom::setAccuracy(float accuracy) {
    _accuracy = accuracy;
    updateTrials();
}

void OddsCalculatorRandom::updateTrials()
{
     _trials = ceil(log(4.0/(1.0-_confidence))/(_accuracy*_accuracy));
}

OddsCalculatorRandom::OddsCalculatorRandom():_confidence(0.99), _accuracy(0.001),_trials(0) {
    updateTrials();
}

std::pair<float, float> OddsCalculatorRandom::handOdds(Card card1, Card card2, Card opp1, Card opp2, Card* street, int street_size) {
    int victories = 0;
    int draws = 0;
    int result = 0;
    
    cout << "Trials: " << _trials << endl;
    _rdeck.removeCard(card1);
    _rdeck.removeCard(card2);
    _rdeck.removeCard(opp1);
    _rdeck.removeCard(opp2);
    
    Card complete_street[5];
    
    for(int i=0; i!=street_size; i++)
    {
        complete_street[i] = street[i];
        _rdeck.removeCard(street[i]);
    }
    
    
    for(int i=0; i!= _trials; i++)
    {
        _rdeck.drawCard(complete_street + street_size, 5-street_size);
        Set s1(complete_street, card1, card2);
        Set s2(complete_street, opp1, opp2);
        if(s1 > s2)
            victories++;
        else if(s1 == s2)
            draws++;
        _rdeck.returnCard(5-street_size);
    }
    
    _rdeck.reset();
    return std::pair<float, float> ((float) victories / (float) _trials, (float) draws / (float) _trials);
}


std::pair<float, float> OddsCalculatorRandom::handOdds(Card card1, Card card2, Card* street, int street_size) {
    return std::pair<float, float> (0.0, 0.0);
}
