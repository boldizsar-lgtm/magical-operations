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

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Creature(std::string id, int mana_cost, std::string name, std::vector<std::shared_ptr<Trait>> base_traits,
  int base_attack, int base_health);
  Creature(Creature&) = default;
  ~Creature() = default;

  //Methods
  void setCurrentAttack(int new_attack);
  void setCurrentHealth(int new_health);
  void addTrait(std::shared_ptr<Trait> trait);
  std::string getType() const override;
  std::string getTraitIds() const;
  void printBaseTraitList() const;
  int getBaseAttack() const;
  int getBaseHealth() const;
  std::vector<std::shared_ptr<Trait>> getBaseTraits() const;
  int getAttack() const;
  int getHealth() const;
  std::string getAttackString() const;
  std::string getHealthString() const;
  void printCardLine(int row) const override;
  bool creatureHasTrait(std::string name) const;
};

#endif //CREATURE_HPP
