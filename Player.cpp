// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
#include "Player.h"

using namespace std;
#include <fstream>
#include <cstdlib>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
//helper functions
void printHand(const vector<Card> &hand, const string &name_input)
{
    for (double i = 0; i < hand.size(); i++)
    {
        cout << "Human player " << name_input << " hand: [" << i << "] " << hand[i] << endl;
    }
}
//Implement simple and human
class SimplePlayer : public Player{
 private:
    string name;
    vector<Card> hand;
 public:
    SimplePlayer(string name_in) //constructor for name
        : name(name_in){}

    const string & get_name() const override{
        return name;
    }

    void add_card(const Card &c) override{
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, 
                    string &order_up_suit) const override{

        //round 1
        if (round == 1)
        {
            int face_trump = 0;
            for (double i = 0; i < hand.size(); i++)
            {
                if ((hand[i].is_face() && hand[i].get_suit() == upcard.get_suit()) || hand[i].is_left_bower(upcard.get_suit()))
                    face_trump++;
            }
            if (face_trump > 1)
            {
                order_up_suit == upcard.get_suit();
                return 1;
            }else
                return 0;
        }

        //round two
        if (is_dealer == 0)
        {
            int face_trump_next = 0;
            string suit_next = Suit_next(upcard.get_suit());
            for (double i = 0; i < hand.size(); i++)
            {
                if ((hand[i].is_face() && hand[i].get_suit() == suit_next) || hand[i].is_left_bower(suit_next))
                    face_trump_next++;               
            }
            if (face_trump_next > 0)
            {
                order_up_suit == suit_next;
                return 1;
            }else
                return 0;
        }

        //Screw the dealer
        order_up_suit = Suit_next(upcard.get_suit());
        return 1;
    }

    void add_and_discard(const Card &upcard) override{
        hand.push_back(upcard);
        int lowestCard = 0;
        for (double i = 0; i < hand.size() - 1; i++)
        {
            if (Card_less(hand[i], hand[i+1], upcard.get_suit()))
                lowestCard = i;
            else
                lowestCard = i+1;
        }
        
        hand.erase(hand.begin()+ lowestCard);
    }

    Card lead_card(const string &trump){
        int highestNonTrump = -1;
        int highestTrump = -1;
        Card returnCard = hand[0];
        if (hand.size() == 1) //one card in hand
        {
            hand.pop_back();
            return returnCard;
        }
        for (double i = 0; i < hand.size()-1; i++)
        {
            if (!(Card_less(hand[i], hand[i+1], trump)))
            {
                if (hand[i].is_trump(trump))
                    highestTrump = i;
                else
                    highestNonTrump = i;
            }
            if (Card_less(hand[i], hand[i+1], trump))
            {
                if (hand[i+1].is_trump(trump))
                    highestTrump = i+1;
                else
                    highestNonTrump = i+1;
            }
        }
        if (highestNonTrump == -1)
        {
            returnCard = hand[highestTrump];
            hand.erase(hand.begin()+highestTrump);
            return returnCard;
        }else{
            returnCard = hand[highestNonTrump];
            hand.erase(hand.begin()+highestNonTrump);
            return returnCard;
        }
    }

    Card play_card(const Card &led_card, const string &trump) override{
        bool led_in_hand = 0;
        Card returnCard = hand[0];
        vector<Card> ledCardVec;
        for (double i = 0; i < hand.size(); i++)
        {
            if (led_card.get_suit() == hand[i].get_suit())
            {
                led_in_hand = 1;
                ledCardVec.push_back(hand[i]);
            }
        }
        if (led_in_hand)
        {
            int currentHighest = 0;
            for (double i = 0; i < ledCardVec.size()-1; i++)
            {
                if (ledCardVec[i]<ledCardVec[i+1])
                    currentHighest = i+1;
            }
        return ledCardVec[currentHighest];
        }

        int currentLowest = 0;
        for (double i = 0; i < hand.size()-1; i++)
        {
            if (!(Card_less(hand[i], hand[i+1], led_card, trump)))
                currentLowest = i+1;
        }
        return hand[currentLowest];
    }
};

class HumanPlayer : public Player{
 private:
    string name;
    vector<Card> hand;
 public:
    HumanPlayer(string name_in) //constructor for name
        : name(name_in){}

    const string & get_name() const override{
        return name;
    }

    void add_card(const Card &c) override{
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                          int round, string &order_up_suit) const override{
        vector<Card> copy_hand = hand;                    
        sort(copy_hand.begin(), copy_hand.end());                         
        printHand(copy_hand, get_name());
        cout << "Human player " << get_name() << ", please enter a suit, or \"pass\":" << endl;
        string userInput;
        cin >> userInput;
        if (userInput == "pass")
            return 0;
        else
        {
            order_up_suit = userInput;
            return 1;
        }
    }

    void add_and_discard(const Card &upcard) override{
        vector<Card> copy_hand = hand;                    
        sort(copy_hand.begin(), copy_hand.end()); 
        hand = copy_hand;   
         //printing before picking up card as shown in spec
        printHand(hand, get_name());
        hand.push_back(upcard);
        cout << "Discard upcard: [-1]" << endl;
        cout << "Human player " << get_name() << ", please select a card to discard:" << endl;
        int userInput;
        cin >> userInput;
        if (userInput >= 0 && userInput < 5)
            hand.erase(hand.begin()+userInput);
        else
            hand.erase(hand.begin() + MAX_HAND_SIZE + 1);
        copy_hand = hand;                    
        sort(copy_hand.begin(), copy_hand.end()); 
        hand = copy_hand;   

        cout << endl;
    }

    Card lead_card(const string &trump){
        printHand(hand, get_name());
        cout << "Human player " << get_name() << ", please select a card:" << endl;
        int userInput;
        cin >> userInput;
        return hand[userInput];
    }

    Card play_card(const Card &led_card, const string &trump) override{
        printHand(hand, get_name());
        cout << "Human player " << get_name() << ", please select a card:" << endl;
        int userInput;
        cin >> userInput;
        return hand[userInput];
    }

};


Player * Player_factory(const string &name, const string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  
  if (strategy == "Human")
    return new HumanPlayer(name);
  

  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}

ostream & operator<<(ostream &os, const Player &p)
{
    os << p.get_name();
    return os;
}
