// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
#include "Pack.h"

using namespace std;
#include <fstream>
#include <cstdlib>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>

Pack::Pack()
{
    int i = 0; //index for the card array
    for (int suit = 0; suit < NUM_SUITS; suit++)
    {
        for (int rank = 7; rank < NUM_RANKS; rank++)
        {
            
            cards[i] = Card(RANK_NAMES_BY_WEIGHT[rank], SUIT_NAMES_BY_WEIGHT[suit]);
            i++;
        }
    }
    next = 0;
}

Pack::Pack(istream& pack_input)
{
    string rank;
    string suit;
    string of;

    for (int i = 0; i < 24; i++)
    {
        pack_input >> rank;
        pack_input >> of;
        pack_input >> suit;
        cards[i] = Card(rank, suit);
    }
    next = 0;
}

Card Pack::deal_one()
{
    next++;
    return cards[next - 1];
}

void Pack::reset()
{
    next = 0;
}

void Pack::shuffle()
{
    for (int rounds = 0; rounds < 7; rounds++)
    {
        //Making two stacks of cards bottom(0-11) top(12-23)
        array<Card, PACK_SIZE/2> bottom_stack;
        array<Card, PACK_SIZE/2> top_stack;
        for (int i = 0; i < PACK_SIZE/2; i++) 
        {
            bottom_stack[i] = cards[i];
            top_stack[i] = cards[i + PACK_SIZE/2];
        }
        //Shuffling
        for (int i = 0; i < PACK_SIZE; i+=2)
        {
            cards[i] = top_stack[i/2];
            cards[i+1] = bottom_stack[i/2];
        }
    }
    next = 0;
}

bool Pack::empty() const
{
    return (next == 24);
}

