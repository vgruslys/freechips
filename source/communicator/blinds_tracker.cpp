#include "blinds_tracker.h"

BlindsTracker :: BlindsTracker()
{
    _initialised = 0;
}

BlindsTracker :: ~BlindsTracker()
{
    if (isInitialised() && _total_levels > 0) {
        delete _small_blind;
        delete _big_blind;
    }
}

void BlindsTracker :: initialise(int small_blind, int big_blind, int hands_until_change)
{
    if (isInitialised())
        throw BlindsError("Blinds tracker was already initialised");

    if (small_blind == 10 && big_blind == 20 && hands_until_change == 10) {
        _initialised = 1;
        _counter = 0;
        _hands_until_change = hands_until_change;

        _total_levels = 47;

        _big_blind = new int[_total_levels];
        _small_blind = new int[_total_levels];

        _big_blind[0] = 20; _small_blind[0] = 10;
        _big_blind[1] = 30; _small_blind[1] = 15;
        _big_blind[2] = 40; _small_blind[2] = 20;
        _big_blind[3] = 50; _small_blind[3] = 25;
        _big_blind[4] = 60; _small_blind[4] = 30;
        _big_blind[5] = 80; _small_blind[5] = 40;
        _big_blind[6] = 100; _small_blind[6] = 50;
        _big_blind[7] = 120; _small_blind[7] = 60;
        _big_blind[8] = 160; _small_blind[8] = 80;
        _big_blind[9] = 200; _small_blind[9] = 100;
        _big_blind[10] = 240; _small_blind[10] = 120;
        _big_blind[11] = 300; _small_blind[11] = 150;
        _big_blind[12] = 400; _small_blind[12] = 200;
        _big_blind[13] = 500; _small_blind[13] = 250;
        _big_blind[14] = 600; _small_blind[14] = 300;
        _big_blind[15] = 800; _small_blind[15] = 400;
        _big_blind[16] = 1000; _small_blind[16] = 500;
        _big_blind[17] = 1200; _small_blind[17] = 600;
        _big_blind[18] = 1600; _small_blind[18] = 800;
        _big_blind[19] = 2000; _small_blind[19] = 1000;
        _big_blind[20] = 2400; _small_blind[20] = 1200;
        _big_blind[21] = 3000; _small_blind[21] = 1500;
        _big_blind[22] = 4000; _small_blind[22] = 2000;
        _big_blind[23] = 5000; _small_blind[23] = 2500;
        _big_blind[24] = 6000; _small_blind[24] = 3000;
        _big_blind[25] = 8000; _small_blind[25] = 4000;
        _big_blind[26] = 10000; _small_blind[26] = 5000;
        _big_blind[27] = 12000; _small_blind[27] = 6000;
        _big_blind[28] = 16000; _small_blind[28] = 8000;
        _big_blind[29] = 20000; _small_blind[29] = 10000;
        _big_blind[30] = 24000; _small_blind[30] = 12000;
        _big_blind[31] = 30000; _small_blind[31] = 15000;
        _big_blind[32] = 40000; _small_blind[32] = 20000;
        _big_blind[33] = 50000; _small_blind[33] = 25000;
        _big_blind[34] = 60000; _small_blind[34] = 30000;
        _big_blind[35] = 80000; _small_blind[35] = 40000;
        _big_blind[36] = 100000; _small_blind[36] = 50000;
        _big_blind[37] = 120000; _small_blind[37] = 60000;
        _big_blind[38] = 160000; _small_blind[38] = 80000;
        _big_blind[39] = 200000; _small_blind[39] = 100000;
        _big_blind[40] = 240000; _small_blind[40] = 120000;
        _big_blind[41] = 300000; _small_blind[41] = 150000;
        _big_blind[42] = 400000; _small_blind[42] = 200000;
        _big_blind[43] = 500000; _small_blind[43] = 250000;
        _big_blind[44] = 600000; _small_blind[44] = 300000;
        _big_blind[45] = 800000; _small_blind[45] = 400000;
        _big_blind[46] = 1000000; _small_blind[46] = 500000;

        _final_small_blind = _big_blind[_total_levels-1];
        _final_big_blind = _small_blind[_total_levels-1];
    }
    else {
        _initialised = 1;
        _counter = 0;
        _hands_until_change = hands_until_change;

        _total_levels = 0;

        _final_small_blind = small_blind;
        _final_big_blind = big_blind;
    }
}

int BlindsTracker :: getSmallBlind() const
{
    if (!isInitialised())
        throw BlindsError("Blinds tracker was not initialised");

    int level = _counter / _hands_until_change;
    if (level >= _total_levels)
        return _final_small_blind << (level - _total_levels);
    else
        return _small_blind[level];
}

int BlindsTracker :: getBigBlind() const
{
    if (!isInitialised())
        throw BlindsError("Blinds tracker was not initialised");

    int level = _counter / _hands_until_change;
    if (level >= _total_levels)
        return _final_big_blind << (level - _total_levels);
    else
        return _big_blind[level];
}

void BlindsTracker :: advance()
{
    ++_counter;
}

int BlindsTracker :: isInitialised() const
{
    return _initialised;
}
