// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"

using namespace std;
#include <fstream>
#include <cstdlib>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

// add your code below

/*
class Card {
    public:
    static constexpr const char* const RANK_TWO = "Two";
    static constexpr const char* const RANK_THREE = "Three";
    static constexpr const char* const RANK_FOUR = "Four";
    static constexpr const char* const RANK_FIVE = "Five";
    static constexpr const char* const RANK_SIX = "Six";
    static constexpr const char* const RANK_SEVEN = "Seven";
    static constexpr const char* const RANK_EIGHT = "Eight";
    static constexpr const char* const RANK_NINE = "Nine";
    static constexpr const char* const RANK_TEN = "Ten";
    static constexpr const char* const RANK_JACK = "Jack";
    static constexpr const char* const RANK_QUEEN = "Queen";
    static constexpr const char* const RANK_KING = "King";
    static constexpr const char* const RANK_ACE = "Ace";

    static constexpr const char* const SUIT_SPADES = "Spades";
    static constexpr const char* const SUIT_HEARTS = "Hearts";
    static constexpr const char* const SUIT_CLUBS = "Clubs";
    static constexpr const char* const SUIT_DIAMONDS = "Diamonds";


};
*/

Card::Card()
{
    rank = RANK_TWO;
    suit = SUIT_SPADES;
}

Card::Card(const string &rank_in, const string &suit_in)
{
    rank = rank_in;
    suit = suit_in;
}

string Card::get_rank() const
{
    return this->rank;
}

string Card::get_suit() const
{
    return this->suit;
}

string Card::get_suit(const string &trump) const
{
    if (this->rank == Card::RANK_JACK && Suit_next(this->suit) == trump) //accounts for left bower
    {
        return trump;
    }else
        return this->suit;
}

bool Card::is_face() const
{
    if (this->suit == RANK_JACK || this->suit == RANK_QUEEN || this->suit == RANK_KING || this->suit == RANK_ACE)
        return 1;
    else 
        return 0;
}

bool Card::is_right_bower(const string &trump) const
{
    if (this->suit == trump && this->rank == RANK_JACK)
        return 1;
    else 
        return 0;
}

bool Card::is_left_bower(const string &trump) const
{
    if (Suit_next(this->suit) == trump && this->rank == RANK_JACK)
        return 1;
    else
        return 0;
}

bool Card::is_trump(const string &trump) const
{
    if (this->suit == trump || is_left_bower(trump))
        return 1;
    else
        return 0;
}

string Suit_next(const string &suit) 
{
    if (suit == Card::SUIT_SPADES)
        return Card::SUIT_CLUBS;
    if (suit == Card::SUIT_CLUBS)
        return Card::SUIT_SPADES;
    if (suit == Card::SUIT_HEARTS)
        return Card::SUIT_DIAMONDS;
    else
        return Card::SUIT_HEARTS;
}

// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec (see the appendices) before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=

bool operator<(const Card &lhs, const Card &rhs)
{    
    int lhs_rank;
    int lhs_suit;
    int rhs_rank;
    int rhs_suit;
    for (int i = 0; i < NUM_RANKS; i++)//loop to assign value to the rank
    {
        if (RANK_NAMES_BY_WEIGHT[i] == lhs.get_rank())
            lhs_rank = i;
        if (RANK_NAMES_BY_WEIGHT[i] == rhs.get_rank())
            rhs_rank = i;
    }
    if (!(lhs_rank == rhs_rank)) //if not equal compare rank
        return lhs_rank < rhs_rank;
    for (int i = 0; i < NUM_SUITS; i++)//loop to assign value to the suit
    {
        if (SUIT_NAMES_BY_WEIGHT[i] == lhs.get_suit())
            lhs_suit = i;
        if (SUIT_NAMES_BY_WEIGHT[i] == rhs.get_suit())
            rhs_suit = i;
    }
    return lhs_suit < rhs_suit;
}   

bool operator==(const Card &lhs, const Card &rhs){
    return lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit();
}

bool operator<=(const Card &lhs, const Card &rhs)
{
    return lhs < rhs || lhs == rhs;
}

bool operator>(const Card &lhs, const Card &rhs)
{
    return !(lhs <= rhs);
}

bool operator>=(const Card &lhs, const Card &rhs)
{
    return !(lhs < rhs);
}


bool operator!=(const Card &lhs, const Card &rhs)
{
    return !(lhs == rhs);
}

ostream & operator<<(ostream &os, const Card &card)
{
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

bool Card_less(const Card &a, const Card &b, const string &trump)
{
    /*
    if (!(a.is_trump(trump) || b.is_trump(trump))) //both not trump
        return a < b;
    */
    if (a.is_trump(trump) && !(b.is_trump(trump)))//a is trump but b isn't
        return 0;
    if (b.is_trump(trump) && !(a.is_trump(trump)))//b is trump but a isn't
        return 1;
    //both trump cases below
    if (a.is_right_bower(trump))
        return 0;
    if (b.is_right_bower(trump))
        return 1;
    if (a.is_left_bower(trump) && !(b.is_right_bower(trump)))
        return 0;
    if (b.is_left_bower(trump) && !(a.is_right_bower(trump)))
        return 1;
  
    return a < b;
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, const string &trump)
{
    string led_suit = led_card.get_suit();

    if (a.get_suit() == led_suit && !(a.get_suit() == trump))
    {
        if (b.is_trump(trump))
            return 1;
        /*
        if (b.get_suit() == led_suit)
            return a < b;
        */
        if (!(b.get_suit() == trump) && !(b.get_suit() == led_suit))
            return 0;
    }

    if (b.get_suit() == led_suit && !(b.get_suit() == trump))
    {
         if (a.is_trump(trump))
            return 0;
        if (!(a.get_suit() == trump) && !(a.get_suit() == led_suit))
            return 1;
    }
    return Card_less(a, b, trump);
}