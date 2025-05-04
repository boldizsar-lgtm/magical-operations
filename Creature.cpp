#include "Creature.hpp"

Creature::Creature(std::string id, int mana_cost, std::string name, std::vector<std::shared_ptr<Trait>> base_traits,
int base_attack, int base_health) : Card(id, mana_cost, name), base_traits_(base_traits), current_traits_(base_traits),
base_attack_(base_attack), current_attack_(base_attack), base_health_(base_health), current_health_(base_health),
can_attack_(false)
{
}

void Creature::setCurrentAttack(int new_attack)
{
  current_attack_ = new_attack;
}

void Creature::setCurrentHealth(int new_health)
{
  current_health_ = new_health;
}

void Creature::addTrait(std::shared_ptr<Trait> trait)
{
  //TODO
}

std::string Creature::getType() const
{
  return "Creature";
}

std::string Creature::getCardString(int row) const
{
  //TODO
}
