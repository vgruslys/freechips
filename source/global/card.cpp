#include "card.h"

const Card HEIGHT_MASK = 0x0f;
const Card SUIT_MASK   = 0x30;

Card strToCard(char* s)
{
    Card height, suit;
    if (s[0] == '2') height = 2;
    else if (s[0] == '3') height = 3;
    else if (s[0] == '4') height = 4;
    else if (s[0] == '5') height = 5;
    else if (s[0] == '6') height = 6;
    else if (s[0] == '7') height = 7;
    else if (s[0] == '8') height = 8;
    else if (s[0] == '9') height = 9;
    else if (s[0] == 'T') height = 10;
    else if (s[0] == 'J') height = 11;
    else if (s[0] == 'Q') height = 12;
    else if (s[0] == 'K') height = 13;
    else if (s[0] == 'A') height = 14;
    else
        throw CardError("Unrecognised card height");

    if (s[1] == 'h') suit = 0;
    else if (s[1] == 'd') suit = 1;
    else if (s[1] == 'c') suit = 2;
    else if (s[1] == 's') suit = 3;
    else
        throw CardError("Unrecognised card suit");

    return (suit << 4) | height;
}

char heightToChar(Height height)
{
    switch (height) {
        
        case 2 : 
            return '2';
            break;
        case 3 : 
            return '3';
            break;
        case 4 : 
            return '4';
            break;
        case 5 : 
            return '5';
            break;
        case 6 : 
            return '6';
            break;
        case 7 : 
            return '7';
            break;
        case 8 : 
            return '8';
            break;
        case 9 : 
            return '9';
            break;
        case 10 : 
            return 'T';
            break;
        case 11 : 
            return 'J';
            break;
        case 12 : 
            return 'Q';
            break;
        case 13 : 
            return 'K';
            break;
        case 14 :
            return 'A';
            break;
        default :
            fprintf(stderr, "Peculiar card height: %d\n", height);
            return '-';
            //throw CardError("Invalid card height");
    }
}

int isAllowedCharForCard(char c)
{
    return (c >= '2' && c <= '9') || (c == 'T') || (c == 'J') || (c == 'Q') || (c == 'K') || (c == 'A') ||
           (c == 'h') || (c == 'd') || (c == 'c') || (c == 's');
}

int parseCards(char* string, Card* cards)
{
    fprintf(stderr, "Parsing this string for cards: %s\n", string);
    fflush(stderr);

    int i = 0;

    for (i = 0; string[i] != ']'; ++i) {

        if (i == 0 && string[i] != '[')
            throw CardError("Parsing badly formatted string");
        
        if (i > 0 && i % 3 == 0 && string[i] != ',' && string[i] != ']')
            throw CardError("Parsing badly formatted string");

        if (i % 3 != 0 && !isAllowedCharForCard(string[i]))
            throw CardError("Parsing badly formatted string");

        if (i > 60 && !isAllowedCharForCard(string[i]))
            throw CardError("Parsing badly formatted string");
    }

    if (i % 3 != 0)
        throw CardError("Parsing badly formatted string");

    int total_cards = i / 3;
    fprintf(stderr, "Found %d cards\n", total_cards);
    fflush(stderr);
    if (total_cards < 1 || total_cards > 5)
        throw CardError("Invalid number of cards");

    for (int j = 0; j < total_cards; ++j)
        cards[j] = strToCard(&string[j*3+1]);


    return total_cards;
}

Height getHeight(Card card)
{
    return card & HEIGHT_MASK; 
}

Suit getSuit(Card card)
{
    return (card & SUIT_MASK) >> 4;
}
