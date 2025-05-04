#include "Spell.hpp"

std::string Spell::getType() const
{
  return "Spell";
}

std::string Spell::getCardString(int row) const
{
  if (row == 1)
    std::cout << " _____M" << Card::getManaCost();
  else if (row == 2)
    std::cout << "| " << Card::getId() << " |";
  else if (row == 3)
    std::cout << "|       |";
  else if (row == 4)
    std::cout << "_______ ";
}
