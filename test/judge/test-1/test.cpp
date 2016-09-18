#include "io/settings.h"

#include <cstdint>
#include <cstdio>

const char* PATH_IN  = "io/test.in";
const char* PATH_OUT = "io/test.out";

const char* card_names[52] = {
    "2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks", "As",
    "2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc", "Qc", "Kc", "Ac",
    "2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d", "Td", "Jd", "Qd", "Kd", "Ad",
    "2h", "3h", "4h", "5h", "6h", "7h", "8h", "9h", "Th", "Jh", "Qh", "Kh", "Ah" };

extern void printBytes(uint64_t);
//void printBytes(uint64_t mask) {
//    for (int i = 63; i >= 0; --i)
//        if (mask & (1LL << i))
//            printf("1");
//        else
//            printf("0");
//    printf("\n");
//}

uint64_t* readInput(FILE* file_in, uint64_t* size)
{
    fread(size, sizeof(uint64_t), 1, file_in);
    uint64_t* data = new uint64_t[*size];
    fread(data, sizeof(uint64_t), *size, file_in);
    return data;
}

int evaluate(uint64_t cards, JUDGE& judge, Community& community, PlayerCards& player_cards)
{
    
    community.reset();
    player_cards.reset();

    community.addCard(    cards & 0x000000000000ffLL      );
    community.addCard(   (cards & 0x0000000000ff00LL) >> 8);
    community.addCard(   (cards & 0x00000000ff0000LL) >> 16);
    community.addCard(   (cards & 0x000000ff000000LL) >> 24);
    community.addCard(   (cards & 0x0000ff00000000LL) >> 32);
    player_cards.addCard((cards & 0x00ff0000000000LL) >> 40);
    player_cards.addCard((cards & 0xff000000000000LL) >> 48);

    return judge.verdict(community, player_cards);
}

int main()
{
    JUDGE judge;
    Community community;
    PlayerCards player_cards;

    FILE* file_in = fopen(PATH_IN, "rb");
    uint64_t size;
    uint64_t* data = readInput(file_in, &size);
    fclose(file_in);

    uint32_t* verdict = new uint32_t[size];
    for (uint64_t i = 0; i < size; ++i) {
        verdict[i] = evaluate(data[i], judge, community, player_cards);    
        /*
        if (i < 10) {
            printf("Case #%lld:\n", i+1);
            printf("[%s,%s,%s,%s,%s|%s,%s]\n",
                    card_names[ data[i] & 0x000000000000ffLL],
                    card_names[(data[i] & 0x0000000000ff00LL) >> 8],
                    card_names[(data[i] & 0x00000000ff0000LL) >> 16],
                    card_names[(data[i] & 0x000000ff000000LL) >> 24],
                    card_names[(data[i] & 0x0000ff00000000LL) >> 32],
                    card_names[(data[i] & 0x00ff0000000000LL) >> 40],
                    card_names[(data[i] & 0xff000000000000LL) >> 48]);
            printf("verdict = %x\n", verdict[i]);
            printBytes(community.getKey());
            printBytes(player_cards.getKey());
        }
        */
    }

    FILE* file_out = fopen(PATH_OUT, "wb");
    fwrite(&size, sizeof(uint32_t), 1, file_out);
    fwrite(verdict, sizeof(uint32_t), size, file_out);
    fclose(file_out);

    delete[] verdict;
    delete[] data;

    return 0;
}
