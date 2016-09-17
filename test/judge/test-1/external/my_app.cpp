#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ace_eval.h"

Card hand[ACEHAND]={0};
Card deck[52];

const char* PATH_IN  = "../io/test.in";
const char* PATH_OUT = "../io/test.sol";

uint64_t* readInput(FILE* file_in, uint64_t* size)
{
    fread(size, sizeof(uint64_t), 1, file_in);
    uint64_t* data = new uint64_t[*size];
    fread(data, sizeof(uint64_t), *size, file_in);
    return data;
}

int evaluate(uint64_t cards)
{
    memset(hand,0,sizeof(Card)*ACEHAND);
    
    ACE_addcard(hand, deck[ cards & 0x000000000000ffLL]);
    ACE_addcard(hand, deck[(cards & 0x0000000000ff00LL) >> 8]);
    ACE_addcard(hand, deck[(cards & 0x00000000ff0000LL) >> 16]);
    ACE_addcard(hand, deck[(cards & 0x000000ff000000LL) >> 24]);
    ACE_addcard(hand, deck[(cards & 0x0000ff00000000LL) >> 32]);
    ACE_addcard(hand, deck[(cards & 0x00ff0000000000LL) >> 40]);
    ACE_addcard(hand, deck[(cards & 0xff000000000000LL) >> 48]);

	return ACE_evaluate( hand );
}

int main()
{
    for (int i = 0; i < 52; ++i)
        deck[i] = ACE_makecard(i);

    FILE* file_in = fopen(PATH_IN, "rb");
    uint64_t size;
    uint64_t* data = readInput(file_in, &size);
    fclose(file_in);

    uint32_t* verdict = new uint32_t[size];
    for (uint64_t i = 0; i < size; ++i)
        verdict[i] = evaluate(data[i]);    

    FILE* file_out = fopen(PATH_OUT, "wb");
    fwrite(&size, sizeof(uint32_t), 1, file_out);
    fwrite(verdict, sizeof(uint32_t), size, file_out);
    fclose(file_out);

    delete[] verdict;
    delete[] data;

    return 0;
}
