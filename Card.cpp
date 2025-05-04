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
