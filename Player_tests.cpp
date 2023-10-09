// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

//public tests
TEST(test_player_insertion) {
  Player * human = Player_factory("NotRobot", "Human");
  ostringstream oss;
  oss << * human;
  ASSERT_EQUAL(oss.str(), "NotRobot");
  oss.str("");
  Player * alice = Player_factory("Alice", "Simple");
  oss << *alice;
  ASSERT_EQUAL(oss.str(), "Alice");
  delete alice;
  delete human;
}

static void add_cards(Player *player) {
  Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
  for (int i = 0; i < 5; ++i) {
    player->add_card(two_spades);
  }
}

TEST(test_simple_player_make_trump) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);

  Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
  string trump = Card::SUIT_SPADES;
  bool orderup = bob->make_trump(
      nine_spades,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump           //suit ordered up (if any)
  );
  ASSERT_FALSE(orderup);
  ASSERT_EQUAL(trump, Card::SUIT_SPADES);

  delete bob;
}

TEST(test_simple_player_lead_card) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);
  Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
  bob->add_and_discard(nine_spades);
  Card card_led = bob->lead_card(Card::SUIT_SPADES);
  ASSERT_EQUAL(card_led, nine_spades); //check led card
  delete bob;
}

TEST(test_simple_player_play_card) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);
  Card nine_diamonds(Card::RANK_NINE, Card::SUIT_DIAMONDS);
  string trump = Card::SUIT_HEARTS;
  Card card_played = bob->play_card(nine_diamonds, trump); //Diamonds is led
  Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
  ASSERT_EQUAL(card_played, two_spades);
  delete bob;
}
//my tests





TEST_MAIN()
