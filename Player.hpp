#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include "Utils.hpp"
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
  CardPile* deck_;
  CardPile graveyard_;

  int base_health_;
  int mana_pool_;

  int current_health_;
  int current_mana_;

  Zone field_zone_;
  Zone battle_zone_;

  bool redraw_possible_;
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
  void playCreature(Card* card, std::string field_slot);
  void addToFirstEmptyField(Creature* creature);
  void moveToGraveyard(char zone, int column);
  void moveToGraveyard(Creature* creature);
  void removeCardFromPile(PileType pile_type, std::string id);
  void moveToBattle(std::string field_slot, std::string battle_slot);
  void moveToField(int battle_column);
  void moveToField(Creature* creature);
  void moveToHand(Creature* creature);
  bool isFieldSlotOccupied(std::string field_slot);
  bool isZoneFull(char zone_type);

  std::string getRoleString() const;
  int getCurrentHealth() const;
  int getCurrentMana() const;
  int getManaPool() const;
  int getId() const;
  int getDeckSize() const;
  CardPile* getDeck() const;
  const CardPile& getGraveyard() const;
  const CardPile& getHand() const;
  bool getRedrawPossible();
  bool getDoneStatus() const;
  const Zone& getZone(char type) const;
  const Zone& getZone(std::string type) const;

  void setRole(Role role);
  void switchRole();
  void turnAllEnteredOff();
  void setRedrawNotPossible();
  void setDoneStatus(bool status);
};

#endif //PLAYER_HPP
