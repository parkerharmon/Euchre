// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Card.h"
#include "Player.h"
#include "Pack.h"
using namespace std;

class Game {
 private: 
    //game data
    vector<Player*> players;
    int hand = 0;
    int score_team_0 = 0;
    int score_team_1 = 0;
    

    //input parameters
    Pack Deck;
    string shuffleYN; //shuffle or noshuffle
    int POINTS_TO_WIN;
 
 public:
    //constructor
    Game(const Pack &d,const string &s,const string &p_in,vector<Player*> &play)
    {
        Deck = d;
        shuffleYN = s; //shuffle or noshuffle
        POINTS_TO_WIN = stoi(p_in); //makes string to int
        vector<Player*> players = play;
    }

   

    void shuffle(string shuffle, Pack &deck_in)
    {
        if (shuffle == "shuffle")
        {
            deck_in.shuffle();
        }else
            deck_in.reset();
    }
    
    //deals players cards and returns upcard
    Card dealCards(int dealer, vector<Player*> &players, Pack &Deck) 
    {
        for (int i = 1; i < 5; i++)
        {
            if (i%2 == 1) //dealing 3 cards (1,3 batches)
            {
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
            }
            if (i%2 == 0) //dealing 2 cards (2,4 batches)
            {
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
            }
        }
        for (int i = 1; i < 5; i++)
        {
            if (i%2 == 0) //dealing 3 cards (6,8 batches)
            {
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
            }
            if (i%2 == 1) //dealing 2 cards (5,7 batches)
            {
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
                players[(dealer + i) % 4]->add_card(Deck.deal_one());
            }
        }
        return Deck.deal_one(); //returning upcard
    }
   
    //loops through players to decide the trump suit;
    string makeTrump(Card upcard, int dealer, vector<Player*> &players, int &teamMadeTrump)
   {
        int round = 1;
        string orderUpTrump;
        bool madeTrump = 0;
        for (int i = 1; i < 9; i++)
        {
            if (i % 4 == dealer) //checking if dealer
            {
                madeTrump = players[(dealer + i) % 4]->make_trump(upcard, 1, round, orderUpTrump);
                if (madeTrump)
                {
                    players[(dealer + i) % 4]->add_and_discard(upcard);
                    teamMadeTrump = ((dealer + i) % 4) % 2;
                    cout << players[(dealer + i) % 4] << " orders up " << upcard.get_suit() << endl;
                }
                else
                    cout << players[(dealer + i) % 4]->get_name() << " passes" << endl;
            }else{
                madeTrump = players[(dealer + i) % 4]->make_trump(upcard, 0, round, orderUpTrump);
                if (madeTrump)
                {
                    players[(dealer + i) % 4]->add_and_discard(upcard);
                    teamMadeTrump = ((dealer + i) % 4) % 2;
                    cout << players[(dealer + i) % 4] << " orders up " << upcard.get_suit() << endl;
                }
                else   
                    cout << players[(dealer + i) % 4]->get_name() << " passes" << endl;
            }
            //check makeTrump
            if (madeTrump == 1)
                return orderUpTrump;

            if (i == 4)
                round = 2;
        }   
        return 0; 
   }

    //build vector of cards played in trick and prints output
    vector<Card> buildCards(vector<Player*> &players,string trump,int leadPlayer)
    {
        vector<Card> cardsPlayed;
      
        Card led = players[(leadPlayer)%4]->lead_card(trump);
        cout << led.get_rank() << " of " << led.get_suit() << " led by " << players[(leadPlayer)%4]->get_name() << endl;
        cardsPlayed.push_back(led);

        Card played = players[(leadPlayer+1)%4]->play_card(led, trump);
        cout << played.get_rank() << " of " << played.get_suit() << " played by " << players[(leadPlayer+1)%4]->get_name() << endl;
        cardsPlayed.push_back(played);

        played = players[(leadPlayer+2)%4]->play_card(led, trump);
        cout << played.get_rank() << " of " << played.get_suit() << " played by " << players[(leadPlayer+2)%4]->get_name() << endl;
        cardsPlayed.push_back(played);

        played = players[(leadPlayer+3)%4]->play_card(led, trump);
        cout << played.get_rank() << " of " << played.get_suit() << " played by " << players[(leadPlayer+3)%4]->get_name() << endl;
        cardsPlayed.push_back(played);
        
        return cardsPlayed;
    }

    //Calculate which team wins the trick
    int calc_win_trick(vector<Card> &cardsPlayed, int leadPlayer, string trump, vector<Player*> &players)
    {
        Card winningCard = cardsPlayed[0];
        
        for (int i = 0; i < 3; i++)
        {
            if (Card_less(winningCard, cardsPlayed[i+1], cardsPlayed[0], trump))
                winningCard = cardsPlayed[i+1];
        }

        if (winningCard == cardsPlayed[(leadPlayer)%4])
        {
            cout << players[(leadPlayer)%4]->get_name() << " takes the trick" << endl << endl;
            return leadPlayer % 2;
        }
        if (winningCard == cardsPlayed[(leadPlayer+1)%4])
        {
            cout << players[(leadPlayer+1)%4]->get_name() << " takes the trick" << endl << endl;
            return leadPlayer % 2;
        }
        if (winningCard == cardsPlayed[(leadPlayer+2)%4])
        {
            cout << players[(leadPlayer+2)%4]->get_name() << " takes the trick" << endl << endl;
            return leadPlayer % 2;
        }
       
        //winning card == card played [+3]
        cout << players[(leadPlayer+3)%4]->get_name() << " takes the trick" << endl << endl;
        return leadPlayer % 2;
        
    }

    bool playGame()
    {
        while(true) //loops until once of the teams win
        {
            shuffle(shuffleYN, Deck); //deck shuffled
            int dealer = hand % 4;    //dealer set 
            
            int teamMadeTrump;
            Card upcard = dealCards(dealer, players, Deck); //players have hand and we have upcard
            string trump = makeTrump(upcard, dealer, players, teamMadeTrump); //returns order_up_suit
            //initializing lead player
            int leadPlayer = (dealer + 1) % 4;
            
            //print beginning hand stats
            cout << "Hand " << hand << endl;
            cout << players[dealer]->get_name() << " deals" << endl;
            cout << upcard.get_rank() << " of " << upcard.get_suit() << " turned up" << endl;

            int trick_W_0 = 0;
            int trick_W_1 = 0;

            //play hand
            for (int trick = 0; trick < 5; trick++)
            {
               
                vector<Card> cardsPlayed = buildCards(players, trump, dealer);
                
                int team_win_trick = calc_win_trick(cardsPlayed, leadPlayer, trump, players);

                //tally which team won trick
                if (team_win_trick == 0)
                    trick_W_0++;
                if (team_win_trick == 1)
                    trick_W_1++;
            }
            int teamWhoWon;
            

            //print who won
            if (trick_W_0 > 2)
            {   
                teamWhoWon = 0;
               
                cout << players[0]->get_name() << " and " << players[2]->get_name() << " win the hand" << endl;
            }
            if (trick_W_1 > 2)
            {
                teamWhoWon = 1;
              
                cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
            }
            //calc score
            if (teamWhoWon == teamMadeTrump)
            {
                if (trick_W_0 == 3 || trick_W_0 == 4)
                    score_team_0++;
                if (trick_W_0 == 5)
                {
                    score_team_0+=2;
                    cout << "march" << endl;
                }
                if (trick_W_1 == 3 || trick_W_1 == 4)
                    score_team_1++;
                if (trick_W_1 == 5)
                {
                    score_team_1+=2;
                    cout << "march" << endl;
                }
            }else
            {
                if (trick_W_0 > 2)
                {
                    score_team_0+=2;
                    cout << "euchred" << endl;
                }
                if (trick_W_1 > 2)
                {
                    score_team_1+=2;
                    cout << "euchred" << endl;
                }

            }

            //check scores
            if (score_team_0 >= POINTS_TO_WIN)
                return 0;
            if (score_team_1 >= POINTS_TO_WIN)
                return 1;
            
            hand++; //increment hand value, used to decide who dealer is 
        }
    }
    
};


int main(int argc, char *argv[]){
    //converting arguments
    string shuffle = argv[2];
    string name_zero = argv[4];
    string type_zero = argv[5];
    string name_one = argv[6];
    string type_one = argv[7];
    string name_two = argv[8];
    string type_two = argv[9];
    string name_three = argv[10];
    string type_three = argv[11];
    //checking arugements
    /*
    if (argc != 12 || stoi(argv[3]) < 1 || stoi(argv[3]) > 100 || !(shuffle == "shuffle" || shuffle == "noshuffle") || 
        !(type_zero == "Human" || type_zero == "Simple") || !(type_one == "Human" || type_one == "Player") || !(type_two == "Human" || type_two == "Player")
        || !(type_three == "Human" || type_three == "Player")) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    */
    ifstream fin(argv[1]); //opening pack file stream and checking if opened
    if (!fin.is_open())
    {
        cout << "Error opening " << argv[1] << endl;
        return 1;
    }
    Pack deck = Pack(fin); //initializing deck 
    vector<Player*> players;//intializing player vector
    players.push_back(Player_factory(name_zero, type_zero));
    players.push_back(Player_factory(name_one, type_one));
    players.push_back(Player_factory(name_two, type_two));
    players.push_back(Player_factory(name_three, type_three));
    Game Euchre(deck, shuffle, argv[3], players);
    
    if (Euchre.playGame())
        cout << name_one << " and " << name_three << " win!";
    else
        cout << name_zero << " and " << name_two << " win!";
    return 0;
}