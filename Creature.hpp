//---------------------------------------------------------------------------------------------------------------------
// Creature.hpp/Creature.cpp
//
// The Creature class contains base and current health/attack values and traits and methods to modify and print these.
//
// Group: 026
//
// Author: Boldizsar Kiss, 12417350,
//         Lenon Osmani, 12303674,
//         Katharina Hartmann, 12219843
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Card.hpp"
#include "Trait.hpp"

class Creature : public Card
{
protected:
  int base_attack_;
  int base_health_;
  std::vector<std::shared_ptr<Trait>> base_traits_;

  int current_attack_;
  int current_health_;
  std::vector<std::shared_ptr<Trait>> current_traits_;

  bool entered_this_turn_;
  bool is_dead_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Creature(std::string id, int mana_cost, std::string name, std::vector<std::shared_ptr<Trait>> base_traits,
  int base_attack, int base_health);
  Creature(Creature&) = default;
  ~Creature() = default;

  //------------------------------------------------------------------------------------------------------------------
  // adds a trait to the current traits
  //
  // @param trait, trait to be added
  //
  Creature& operator+=(const std::shared_ptr<Trait> trait);


  //------------------------------------------------------------------------------------------------------------------
  // sets the creature's current attack to new_attack
  //
  // @param new_attack, the new attack value of the creature
  //
  void setCurrentAttack(int new_attack);

  //------------------------------------------------------------------------------------------------------------------
  // sets the creature's current health to new_health
  //
  // @param new_health, the new health value of the creature
  //
  void setCurrentHealth(int new_health);

  //------------------------------------------------------------------------------------------------------------------
  // sets the creature's dead status to status
  //
  // @param status, the new dead status
  //
  void setDeadStatus(bool status);

  //------------------------------------------------------------------------------------------------------------------
  // adds "amount" to the creature's current attack (if amount is negative, it is subtracted)
  //
  // @param amount, the number to be added or subtracted
  //
  void changeCurrentAttack(int amount);

  //------------------------------------------------------------------------------------------------------------------
  // adds "amount" to the creature's current health (if amount is negative, it is subtracted)
  //
  // @param amount, the number to be added or subtracted
  //
  void changeCurrentHealth(int amount);

  //------------------------------------------------------------------------------------------------------------------
  // adds a specific trait to the creature's current_traits_, if it doesn't already haave that trait
  // the trait is added in alphabetical order
  //
  // @param trait, a shared pointer of a trait to be added
  //
  void addTrait(std::shared_ptr<Trait> trait);

  //------------------------------------------------------------------------------------------------------------------
  // removes a specific trait from the creature's current_traits_
  //
  // @param trait, a string of the trait's name that should be removed
  //
  void removeTrait(std::string trait);

  //------------------------------------------------------------------------------------------------------------------
  // removes the first trait from the creautre's current_traits_, if it has any
  // (traits are sorted in alphabetical order)
  //
  void removeFirstTrait();

  //------------------------------------------------------------------------------------------------------------------
  // returns the type of the card as a string
  //
  // @return "Creature"
  //
  std::string getType() const override;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature's current traits as a string of their ids
  // if the creature has over 5 traits it omits any trait after the fourth
  //
  // @return "HT   ", if the creature has Haste, and Trample
  //         "BCFH+" if the creature has Brutal, Challenger, First Strike, Haste, Lifesteal, Poisoned
  //
  std::string getTraitIds() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns dead status of the creature
  //
  // @return dead_status_ member variable
  //
  bool getDeadStatus();

  //------------------------------------------------------------------------------------------------------------------
  // prints all namess of a creature's base_traits_
  //
  void printBaseTraitList() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature's base_attack_
  //
  // @return base_attack_, an integer
  //
  int getBaseAttack() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature's current_attack_
  //
  // @return current_attack_, an integer
  //
  int getCurrentAttack() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature's base_health_
  //
  // @return base_health_, an integer
  //
  int getBaseHealth() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature's current_health_
  //
  // @return current_health_, an integer
  //
  int getCurrentHealth() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the base_traits_ vector of shared trait pointers
  //
  // @return base_traits_ vector of shared trait pointers
  //
  std::vector<std::shared_ptr<Trait>> getBaseTraits() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the current_traits_ vector of shared trait pointeers
  //
  // @return current_traits_ vector of shared trait pointeers
  //
  std::vector<std::shared_ptr<Trait>> getCurrentTraits() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature's current_attack_ as a string for the purrpose of printing the card
  //
  // @return "01", if the current_attack_ is 1
  //         "10", if the current_attack_ is 10
  //
  std::string getAttackString() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature's current_health_ as a string for the purrpose of printing the card
  //
  // @return "01", if the current_health_ is 1
  //         "10", if the current_health_ is 10
  //
  std::string getHealthString() const;

  //------------------------------------------------------------------------------------------------------------------
  // prints one of the four lines required to display a single card.
  //
  // @param row - an integer between 1 and 4, dictating which of the four lines should be printed
  //
  void printCardLine(int row) const override;

  //------------------------------------------------------------------------------------------------------------------
  // checks if the creature has a specific trait in current_traits_
  //
  // @param name, the name of the trait to be checked
  //
  // @return true, if the creature has the given trait
  //         false, if it doesn't
  //
  bool creatureHasTrait(std::string name) const;

  //------------------------------------------------------------------------------------------------------------------
  // checks if the creature has a specific trait in current_traits_
  //
  // @return true, if the creature entered the battlefield this turn, and can't attack
  //         false, if it didn't enter this turrn, and it can attack
  //
  bool getEnter() const;

  //------------------------------------------------------------------------------------------------------------------
  // set the creature's entered_this_turn_ bool
  //
  // @param enter_value, true or false, the new value of the bool
  //
  void setEnter(bool enter_value);
};

#endif //CREATURE_HPP
