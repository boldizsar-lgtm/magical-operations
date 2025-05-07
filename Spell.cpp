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

void Spell::execute(const Game& game)
{
  // TODO:
  // the idea is to make this command essentially function like processCommand where we check the spell name/id and then have a lot of further methods
  // (one for each spell e.g. executeFireball(const Game& game)) that execute that specific spell
}

SpellType Spell::getSubtype() const
{
  return subtype_;
}

std::string Spell::getEffect() const
{
  return effect_;
}