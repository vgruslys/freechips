#include "test-1/io/settings.h"

#include "../../source/global/logger.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

uint64_t* readInput(FILE* file_in, uint64_t* size)
{
    fread(size, sizeof(uint64_t), 1, file_in);
    uint64_t* data = new uint64_t[*size];
    fread(data, sizeof(uint64_t), *size, file_in);
    return data;
}

int evaluate(JUDGE& judge, Community& community, PlayerCards& player_cards)
{
    return judge.verdict(community, player_cards);
}

Card reformatCard(Card card)
{
    return (card & 0xf) - 2 + ((card & 0x30) >> 4) * 13;
}

void printBits(uint64_t mask)
{
    for (int i = 63; i >= 0; --i) {
        if (mask & (1LL << i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

void readCommunity(Community& community)
{
    printf("Enter community cards: ");
    char string[256];
    scanf("%s", string);
    Card cards[5];
    int total = parseCards(string, cards);
    if (total != 5) {
        printf("Community needs 5 cards\n");
        exit(0);
    }

    community.reset();
    for (int i = 0; i < 5; ++i) {
        printf("Adding %d\n", reformatCard(cards[i]));
        community.addCard(reformatCard(cards[i]));
    }
}

void readPlayerCards(PlayerCards& player_cards)
{
    printf("Enter player cards: ");
    char string[256];
    scanf("%s", string);
    Card cards[2];
    int total = parseCards(string, cards);
    if (total != 2) {
        printf("Player needs 2 cards\n");
        exit(0);
    }

    player_cards.reset();
    for (int i = 0; i < 2; ++i) {
        printf("Adding %d\n", reformatCard(cards[i]));
        player_cards.addCard(reformatCard(cards[i]));
    }
}

int main()
{
    JUDGE judge;
    Community community;
    PlayerCards player_cards;
    
    readCommunity(community);
    readPlayerCards(player_cards);

    printBits(community.getKey());
    printBits(player_cards.getKey());

    uint32_t verdict = evaluate(judge, community, player_cards);    

    Logger logger(stderr);
    printf("%s\n", logger.handValueToStr(verdict));

    printf("%x\n", verdict);

    return 0;
}
