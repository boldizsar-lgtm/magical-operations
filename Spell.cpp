#include "Spell.hpp"
#include "Game.hpp"

std::string Spell::getType() const
{
  return "Spell";
}

void Spell::printCardLine(int row) const
{
  if (row == 1)
    std::cout << " _____M" << Card::getManaCostString();
  else if (row == 2)
    std::cout << "| " << Card::getId() << " |";
  else if (row == 3)
    std::cout << "|       |";
  else if (row == 4)
    std::cout << " _______ ";
} 

SpellType Spell::getSubtype() const
{
  return subtype_;
}

std::string Spell::getEffect() const
{
  return effect_;
}