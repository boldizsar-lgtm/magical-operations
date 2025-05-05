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

std::string Creature::getTraitIds() const
{
  std::string traits_string;
  int letter_amount = 0;
  bool more_than_five = false;

  for (auto trait : current_traits_)
  {
    if (letter_amount == 5)
    {
      more_than_five = true;
      break;
    }

    traits_string += trait->getId();
    letter_amount++;
  }

  if (more_than_five == true)
    traits_string.at(4) = '+';

  else
  {
    while (letter_amount < 5)
    {
      traits_string += ' ';
      letter_amount++;
    }
  }

  return traits_string;
}

void Creature::printBaseTraitList() const
{
  if (base_traits_.empty() == false)
  {
    for (auto trait : base_traits_)
    {
      std::cout << trait->getName();

      if (trait == base_traits_.back())
        break;

      std::cout << ", ";
    }
  }
  else
    std::cout << "-";

  std::cout << std::endl;
}

int Creature::getBaseAttack() const
{
  return base_attack_;
}

int Creature::getBaseHealth() const
{
  return base_health_;
}

std::vector<std::shared_ptr<Trait>> Creature::getBaseTraits() const
{
  return base_traits_;
}

int Creature::getAttack() const
{
  return current_attack_;
}

int Creature::getHealth() const
{
  return current_health_;
}

std::string Creature::getAttackString() const
{
  std::string attack_string;

  if (current_attack_ < 10)
    attack_string += '0';

  attack_string += std::to_string(current_attack_);

  return attack_string;
}

std::string Creature::getHealthString() const
{
  std::string health_string;

  if (current_health_ < 10)
    health_string += '0';

  health_string += std::to_string(current_health_);

  return health_string;
}

void Creature::printCardLine(int row) const
{
  if (row == 1)
    std::cout << " _____M" << Card::getManaCostString();
  else if (row == 2)
    std::cout << "| " << Card::getId() << " |";
  else if (row == 3)
    std::cout << "| " << getTraitIds() << " |";
  else if (row == 4)
    std::cout << 'A' << getAttackString() << "___H" << getHealthString();
}