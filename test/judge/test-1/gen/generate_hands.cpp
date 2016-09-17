#include <cstdint>
#include <cstdio>
#include <cstdlib>

const char* PATH_OUT = "../io/test.in";

struct RandomCards {
    RandomCards(int);

    uint64_t sevenCards();
    uint64_t draw();
    void reset();
    
    uint64_t _deck[52];
    uint8_t _available;
};

RandomCards :: RandomCards(int seed)
{
    srand(seed);

    for (uint64_t i = 0; i < 52; ++i)
       _deck[i] = i; 
    _available = 52;
}

uint64_t RandomCards :: sevenCards()
{
    reset();
    uint64_t ans = 0;
    ans |= draw();
    ans |= draw() << 8;
    ans |= draw() << 16;
    ans |= draw() << 24;
    ans |= draw() << 32;
    ans |= draw() << 40;
    ans |= draw() << 48;
    return ans;
}

void RandomCards :: reset()
{
    _available = 52;
}

uint64_t RandomCards :: draw()
{
    uint8_t pick = rand() % _available;
    if (pick < _available - 1) {
        uint64_t temp = _deck[pick];
        _deck[pick] = _deck[_available-1];
        _deck[_available-1] = temp;
    }
    --_available;
    return _deck[_available];
}

int main()
{
    RandomCards random_cards(0);

    FILE* file_out = fopen(PATH_OUT, "wb");
    uint64_t size = 20000000;
    fwrite(&size, sizeof(uint64_t), 1, file_out);
    
    uint64_t* data = new uint64_t[size];
    for (uint64_t i = 0; i < size; ++i)
        data[i] = random_cards.sevenCards();
    fwrite(data, sizeof(uint64_t), size, file_out);

    delete[] data;
    fclose(file_out);

    return 0;
}
