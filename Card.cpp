#include "Card.hpp"

std::string Card::getId() const
{
  return id_;
}

std::string Card::getName() const
{
  return name_;
}

int Card::getManaCost() const
{
  return mana_cost_;
}

std::string Card::getManaCostString() const
{
  //x costed spells are saved with -1 mana cost
  if (mana_cost_ == -1)
    return "XX";
  
  std::string mana_cost_string;

  //add 0 before the cost if it is single digit
  if (mana_cost_ < 10)
    mana_cost_string += '0';

  mana_cost_string += std::to_string(mana_cost_);

  return mana_cost_string;
}

