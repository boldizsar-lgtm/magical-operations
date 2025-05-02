#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <vector>
#include <string>
#include <memory>
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

  bool can_attack_;
  
public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Creature(std::string id, int mana_cost, std::string name, std::vector<std::shared_ptr<Trait>> base_traits,
           int base_attack, int base_health) : Card(id, mana_cost, name), base_traits_(base_traits),
           current_traits_(base_traits) base_attack_(base_attack), current_attack_(base_attack),
           base_health_(base_health), current_health_(base_health), can_attack_(false) {}
  Creature(Creature&) = default;
  ~Creature() = default;

  //Methods
  void setCurrentAttack(int new_attack);
  void setCurrentHealth(int new_health);
  void addTrait(Trait* trait);
  std::string getCardString(int row) const override;
};

#endif //CREATURE_HPP
