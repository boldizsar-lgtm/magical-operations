#include "Zone.hpp"

void Zone::removeCreature(std::string id)
{
  int index{0};
  for (auto& creature : slots_)
  {
    if (creature != nullptr)
    {
      if (creature->getId() == id)
      {
        slots_[index] = nullptr;
        return;
      }
      index++;
    }
  }
}

void Zone::addCreature(Creature* creature, int column)
{
  // *** STILL NEED TO MAKE IT SO WHEN A CREATURE ENTERS THE BOARD, IT'S CURRENT VALUES ARE SET TO ITS BASE VALUES, PROBABLY BEST DONE IN addCreature() ***
  slots_[column - 1] = creature;
}

void Zone::printZone(int row) const
{
  for (auto& creature : slots_)
  {
    if (creature == nullptr)
      std::cout << "         ";
    else
      creature->printCardLine(row);

    std::cout << "   ";
  }
}

bool Zone::isOccupied(int column)
{
  if (slots_[column - 1] != nullptr)
    return true;
  else
    return false;
}

Zone::~Zone()
{
  for(auto& slot : slots_)
  {
    if (slot != nullptr)
    {
      delete slot;
    }
  }
}
