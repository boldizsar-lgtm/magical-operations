//---------------------------------------------------------------------------------------------------------------------
// Player.hpp/Player.cpp
//
// The Player class contains all relevant information on the players of the current game and methods to modify current
// health/attack values, the zones of each player and therefore creature member values and card piles of each player.
//
// Group: 026
//
// Author: Boldizsar Kiss, 12417350,
//         Lenon Osmani, 12303674,
//         Katharina Hartmann, 12219843
//---------------------------------------------------------------------------------------------------------------------
//

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

  //------------------------------------------------------------------------------------------------------------------
  // adds "amount" to the player's curren_health_ (if amount is negative, it is subtracted)
  //
  // @param amount, the number to be added or subtracted
  //
  void changeCurrentHealth(int amount);

  //------------------------------------------------------------------------------------------------------------------
  // adds "amount" to the player's curren_mana_ (if amount is negative, it is subtracted)
  //
  // @param amount, the number to be added or subtracted
  //
  void changeCurrentMana(int amount);

  //------------------------------------------------------------------------------------------------------------------
  // refills the players current_mana_ to the same amount as their mana_pool_
  //
  void refillCurrentMana();

  //------------------------------------------------------------------------------------------------------------------
  // adds "amount" to the player's mana_pool_ (if amount is negative, it is subtracted)
  //
  // @param amount, the number to be added or subtracted
  //
  void increaseManaPool(int amount);

  //------------------------------------------------------------------------------------------------------------------
  // adds a card from the players deck to their hand and removes it from the deck, if the deck has any cards in it.
  //
  // @return true, if a card was successfully drawn
  //         false, if the player's deck is empty
  //
  bool drawCard();

  //------------------------------------------------------------------------------------------------------------------
  // prints the promt that signals the player to input a command
  //
  void printPlayerPrompt() const;

  //------------------------------------------------------------------------------------------------------------------
  // prints the player's graveyard
  //
  void printGraveyard() const;

  //------------------------------------------------------------------------------------------------------------------
  // prints the player's hand
  //
  void printHand() const;

  //------------------------------------------------------------------------------------------------------------------
  // prints the player's field zone
  //
  void printFieldZone() const;

  //------------------------------------------------------------------------------------------------------------------
  // prints the player's battle zone
  //
  void printBattleZone() const;

  //------------------------------------------------------------------------------------------------------------------
  // redraws the player's hand by putting their hand cards at the bottom of their deck, and drawing one less card than
  // the previous number of hand cards. Only possible if redraw_possible_ is true, which is turned to false after
  // specific commands have been used by the player,  and if the player has mor than 2 hand cards
  //
  void redrawHand();

  //------------------------------------------------------------------------------------------------------------------
  // puts a creature from a column of a specific zone into the palayer's graveyard
  //
  // @param zone, F or B indiciating Field zone or Battle zone
  // @param column, an integer between 1 and 7, indicating the column of the zone
  //
  void moveToGraveyard(char zone, int column);

  //------------------------------------------------------------------------------------------------------------------
  // searches both zones for a specific creature, removes it from that zone and adds it to the graveyard
  //
  // @param creature, a creature pointer, that gets removed from a zone, and added to the graveyard
  //
  void moveToGraveyard(Creature* creature);

  //------------------------------------------------------------------------------------------------------------------
  // puts a creature from the player's hand into the chosen slot of the field zone
  //
  // @param card, the creature card pointer to be added to the field zone
  // @param field_slot, a string of an integer between 1 and 7 indicating which column the creature enters
  //
  void playCreature(Card* card, std::string field_slot);

  //------------------------------------------------------------------------------------------------------------------
  // puts a creature on the first empty field zone of the player
  //
  // @param card, the creature card pointer to be added to the field zone
  //
  void addToFirstEmptyField(Creature* creature);

  //NEW a3 method
  int getFirstEmptyFieldColumn();

  //------------------------------------------------------------------------------------------------------------------
  // remove a card with a specific id from the player's deck, hand, or graveyard
  //
  // @param pile_type, an enum class PileType that indicates which CardPile the card should be removed from
  // @param id, a string that coresponds to a card's id in that CardPile
  //
  void removeCardFromPile(PileType pile_type, std::string id);

  //------------------------------------------------------------------------------------------------------------------
  // moves a creature from the field zone to the battle zone
  //
  // @param field_slot, a string of an integer between 1 and 7, indicating the column in which the creature
  //                    is originally positioned, in the fiel zone
  // @param battle_slot, a string of an integer between 1 and 7, indicating the column in which the creature
  //                     ends up positioned, in the battle zone
  //
  void moveToBattle(std::string field_slot, std::string battle_slot);

  //------------------------------------------------------------------------------------------------------------------
  // moves a creature from the battle zone to the field zone
  // if no field zone is empty, the creature is moved to the graveyard
  //
  // @param battle_column, an integer between 1 and 7, indicating the column in which the creature is originally
  //                       positionedm in the battle zone
  //
  void moveToField(int battle_column);

  //------------------------------------------------------------------------------------------------------------------
  // moves a creature from the graveyard to the field zone
  //
  // @param creature, a creature pointer that is to be added to the field zone and removed from the graveyard
  //
  void moveToField(Creature* creature);

  //------------------------------------------------------------------------------------------------------------------
  // returns a card from the graveyard to the player's hand, and sets its attributes to their default values
  //
  // @param creature, a creature pointer that is to be added to the player's hand and removed from the graveyard
  //
  void moveToHand(Creature* creature);

  //------------------------------------------------------------------------------------------------------------------
  // checks if a given column of the field zone is occupied
  //
  // @param field_slot, a string of an integer between 1 and 7, indicating the column which is checked
  //
  // @return true, if the slot is occupied (not a nullptr)
  //         false, if the slot is free (a nullptr)
  //
  bool isFieldSlotOccupied(std::string field_slot);

  //------------------------------------------------------------------------------------------------------------------
  // checks if a given zone is fully occupied
  //
  // @param zone_type, a char, either 'F' or 'B', indicating field zone or battle zone
  //
  // @return true, if the slot is occupied (not a nullptr)
  //         false, if the slot is free (a nullptr)
  //
  bool isZoneFull(char zone_type);

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's role as a string
  //
  // @return "Attacker", if the player has the Role ATTACKER
  //         "Defender", if the player has the Role DEFENDER
  //
  std::string getRoleString() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's current health
  //
  // @return current_health_
  //
  int getCurrentHealth() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's current mana
  //
  // @return current_mana_
  //
  int getCurrentMana() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's mana pool (the max mana each player has when their mana resets)
  //
  // @return mana_pool_
  //
  int getManaPool() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's id
  //
  // @return id (1 or 2)
  //
  int getId() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's deck size
  //
  // @return the number of cards that are in the player's deck
  //
  int getDeckSize() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's deck
  //
  // @return a CardPile pointer the player's deck, stored in Config
  //
  CardPile* getDeck() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's graveyard
  //
  // @return a CardPile refrence the player's graveyard
  //
  const CardPile& getGraveyard() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the player's hand
  //
  // @return a CardPile refrence the player's graveyard
  //
  const CardPile& getHand() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns whether or not the player can use the redraw command
  //
  // @return true, if the redraw command is availabble
  //         false, if it has been disabled
  //
  bool getRedrawPossible();

  //------------------------------------------------------------------------------------------------------------------
  // returns whether or not the player has entered done
  //
  // @return true, if the done command was entered by the player
  //         false, if it was not entered
  //
  bool getDoneStatus() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns one of the player's zones
  //
  // @param  type, a char, either 'F' or 'B', for field or battle zone. Determining which zone to returrn.
  //
  // @return a refrence to one of the player's zones
  //
  const Zone& getZone(char type) const;

  //------------------------------------------------------------------------------------------------------------------
  // returns one of the player's zones
  //
  // @param  type, a char, either 'FIELD' or 'BATTLE'. Determining which zone to returrn.
  //
  // @return a refrence to one of the player's zones
  //
  const Zone& getZone(std::string type) const;

  //------------------------------------------------------------------------------------------------------------------
  // sets the player's role
  //
  // @param role, Enum Class Role, the new role of the player (DEFENDER or ATTACKER)
  //
  void setRole(Role role);

  //------------------------------------------------------------------------------------------------------------------
  // switches the player's role, if the player is the attacker, they become the defender, and vice versa
  //
  void switchRole();

  //------------------------------------------------------------------------------------------------------------------
  // sets all "entered_this_turn_" bools of all creatures on all zones to false
  //
  void turnAllEnteredOff();

  //------------------------------------------------------------------------------------------------------------------
  // sets redraw_possible_ to false to desable the player from using the redraw command after specific actions
  // have been performed
  //
  void setRedrawNotPossible();

  //------------------------------------------------------------------------------------------------------------------
  // sets the player'ss done_ attribute, to keep track if the player has ended their turn
  //
  // @param status, true or false, the new value of done_
  //
  void setDoneStatus(bool status);
};

#endif //PLAYER_HPP
