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

std::string Creature::printTraits() const
{
  std::string traits_string;
  for (auto trait : current_traits_)
  {
    traits_string += trait->getId();
  }

  return traits_string;
}

int Creature::getAttack() const
{
  return current_attack_;
}

int Creature::getHealth() const
{
  return current_health_;
}

std::string Creature::getCardString(int row) const
{
  if (row == 1)
    std::cout << " _____M" << Card::getManaCost();
  else if (row == 2)
    std::cout << "| " << Card::getId() << " |";
  else if (row == 3)
    std::cout << "| " << printTraits() << " |";
  else if (row == 4)
    std::cout << 'A' << getAttack() << "___H" << getHealth();
}
