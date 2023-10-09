// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());

    Card jack_heart(Card::RANK_JACK, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::SUIT_DIAMONDS, jack_heart.get_suit(Card::SUIT_DIAMONDS));
}

//public cases
TEST(test_card_suit_and_rank) {
    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), Card::RANK_TWO);
    ASSERT_EQUAL(two_spades.get_suit(), Card::SUIT_SPADES);

    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_EQUAL(three_spades.get_rank(), Card::RANK_THREE);
    ASSERT_EQUAL(three_spades.get_suit(), Card::SUIT_SPADES);
    ASSERT_EQUAL(three_spades.get_suit(Card::SUIT_CLUBS), Card::SUIT_SPADES);
}

TEST(test_card_type) {
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_FALSE(three_spades.is_face());
    ASSERT_FALSE(three_spades.is_right_bower(Card::SUIT_CLUBS));
    ASSERT_FALSE(three_spades.is_left_bower(Card::SUIT_CLUBS));
    ASSERT_FALSE(three_spades.is_trump(Card::SUIT_CLUBS));
}

TEST(test_card_self_comparison) {
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_FALSE(three_spades < three_spades);
    ASSERT_TRUE(three_spades <= three_spades);
    ASSERT_FALSE(three_spades > three_spades);
    ASSERT_TRUE(three_spades >= three_spades);
    ASSERT_TRUE(three_spades == three_spades);
    ASSERT_FALSE(three_spades != three_spades);
}

TEST(test_Suit_next) {
    ASSERT_EQUAL(Suit_next(Card::SUIT_CLUBS), Card::SUIT_SPADES);
    ASSERT_EQUAL(Suit_next(Card::SUIT_SPADES), Card::SUIT_CLUBS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_HEARTS), Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_DIAMONDS), Card::SUIT_HEARTS);
}

TEST(test_Card_less_self) {
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    Card jack_diamond = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    ASSERT_FALSE(Card_less(three_spades, three_spades, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(three_spades, three_spades, three_spades,
                           Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(jack_diamond, jack_diamond, Card::SUIT_DIAMONDS));
    ASSERT_FALSE(Card_less(jack_diamond, jack_diamond, Card::SUIT_HEARTS));
}

TEST(test_card_insertion) {
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Three of Spades");
}
//my own cases
TEST(test_trump_bower_less){
    Card jack_heart = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card jack_diamond = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_TRUE(Card_less(jack_heart, jack_diamond, jack_heart, Card::SUIT_DIAMONDS));
    ASSERT_FALSE(Card_less(jack_heart, jack_diamond, jack_heart, Card::SUIT_HEARTS));
    ASSERT_TRUE(Card_less(jack_heart, jack_diamond, three_spades, Card::SUIT_DIAMONDS));
    ASSERT_FALSE(Card_less(jack_heart, jack_diamond, three_spades, Card::SUIT_HEARTS));
}

TEST(test_Card_less_edge){
    Card jack_diamond = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    Card four_clubs = Card(Card::RANK_FOUR, Card::SUIT_CLUBS);
    ASSERT_FALSE(Card_less(three_spades,three_spades,three_spades, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(three_spades,three_spades,three_spades, Card::SUIT_SPADES));
    ASSERT_FALSE(Card_less(three_spades,three_spades,jack_diamond, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(three_spades,three_spades,jack_diamond, Card::SUIT_SPADES));
    ASSERT_TRUE(Card_less(four_clubs, three_spades, three_spades, Card::SUIT_DIAMONDS))
}

TEST(comparison_basic){
    Card jack_diamond = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card king_diamond = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_TRUE(king_diamond > jack_diamond);
    ASSERT_FALSE(jack_diamond > king_diamond);
    ASSERT_FALSE(jack_diamond == jack_spades);
    ASSERT_FALSE(jack_diamond == king_diamond);
    ASSERT_TRUE(jack_diamond > jack_spades);
    ASSERT_FALSE(jack_diamond < jack_spades);
}


TEST_MAIN()
