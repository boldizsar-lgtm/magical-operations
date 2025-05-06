#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "CardPile.hpp"
#include "Zone.hpp"

enum class Role
{
  ATTACKER,
  DEFENDER
};

class Player
{
private:
  int id_;

  Role role_;

  CardPile hand_;
  CardPile deck_;
  CardPile graveyard_;

  int base_health_;
  int mana_pool_;

  int current_health_;
  int current_mana_;

  Zone field_zone_;
  Zone battle_zone_;

  bool done_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Player(int id);
  Player(Player&) = default;
  ~Player() = default;

  //Methods
  void changeCurrentHealth(int amount);
  void changeCurrentMana(int amount);
  void refillCurrentMana();
  void increaseManaPool(int amount);
  bool drawCard();
  void printPlayerPrompt() const;
  void printGraveyard() const;
  void printHand() const;
  void printFieldZone() const;
  void printBattleZone() const;
  void redrawHand();

  std::string getRoleString() const;
  int getCurrentHealth() const;
  int getCurrentMana() const;
  int getId() const;
  int getDeckSize() const;
  bool getDoneStatus() const;
  void setRole(Role role);
  void switchRole();
  void setDoneStatus(bool status);
};

#endif //PLAYER_HPP
