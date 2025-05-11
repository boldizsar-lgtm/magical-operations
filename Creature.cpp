#include "Creature.hpp"

Creature::Creature(std::string id, int mana_cost, std::string name, std::vector<std::shared_ptr<Trait>> base_traits,
int base_attack, int base_health) : Card(id, mana_cost, name), base_attack_(base_attack), base_health_(base_health),
                                    base_traits_(base_traits), current_attack_(base_attack),
                                    current_health_(base_health), current_traits_(base_traits),
                                    entered_this_turn_(false)
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

void Creature::changeCurrentAttack(int amount)
{
  current_attack_ += amount;
}

void Creature::changeCurrentHealth(int amount)
{
  current_health_ += amount;
}

void Creature::addTrait(std::shared_ptr<Trait> trait)
{
  //If the creature has no traits, just add the trait
  if (current_traits_.empty())
  {
    current_traits_.push_back(trait);
    return;
  }

  int insert_before = 0;
  for (size_t i = 0; i < current_traits_.size(); i++)
  {
    //If the creature already has the trait return
    if (current_traits_[i]->getId() == trait->getId())
      return;
    
    //Save the number of the last trait that comes alphabetically before the one to be added
    //Add 1 to it to save number of the trait that comes after
    if (current_traits_[i]->getId() < trait->getId())
      insert_before = i + 1;
  }

  current_traits_.insert(current_traits_.begin() + insert_before, trait);
}


void Creature::removeTrait(std::string trait)
{
  for (size_t i = 0; i < current_traits_.size(); i++)
  {
    if (current_traits_[i]->getName() == trait)
    {
      current_traits_.erase(current_traits_.begin() + i);
      return;
    }
  }
}

void Creature::removeFirstTrait()
{
  current_traits_.erase(current_traits_.begin());
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

std::vector<std::shared_ptr<Trait>> Creature::getCurrentTraits() const
{
  return current_traits_;
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

bool Creature::creatureHasTrait(std::string name) const
{
  for (auto& t : current_traits_)
    if (t->getName() == name)
      return true;
  
  return false;
}

bool Creature::getEnter() const
{
  return entered_this_turn_;
}

void Creature::setEnter(bool enter_value)
{
  entered_this_turn_ = enter_value;
}