#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "CardPile.hpp"
#include "Zone.hpp"

class Player
{
private:
  CardPile hand_;
  CardPile deck_;
  CardPile graveyard_;

  int base_health_;
  int base_mana_;

  int current_health_;
  int current_mana_;

  Zone field_zone_;
  Zone battle_zone_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Player() = default;
  Player(Player&) = default;
  ~Player() = default;

  //Methods
  void changeCurrentHealth(int amount);
  void changeCurrentMana(int amount);
  void refillCurrentMana();
  void increaseMaxMana();
  void drawCard();
  std::string printGraveyard() const;
  std::string printHand() const;
  void redrawHand();
};

#endif //PLAYER_HPP
