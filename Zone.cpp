#include "Zone.hpp"

const int CREATURE_NOT_FOUND (-1);
const int ZONE_WIDTH (7);

void Zone::removeCreature(int column)
{
  slots_[column - 1] = nullptr;
}

void Zone::addCreature(Creature* creature, int column)
{
  // Set values to default values
  *creature = *dynamic_cast<Creature*>(Config::getInstance().getCard(creature->getId()));

  slots_[column - 1] = creature;
}

void Zone::switchCreaturePlace(Creature* creature, int column)
{
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

bool Zone::isOccupied(int column) const
{
  if (slots_[column - 1] != nullptr)
    return true;
  else
    return false;
}

Creature* Zone::getCreature(int column) const
{
  return slots_[column - 1];
}

std::vector<Creature*> Zone::getSlots() const
{
  return slots_;
}

int Zone::searchCreature(Creature* creature)
{
  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    if (creature == slots_[column - 1])
      return column;
  }
  return CREATURE_NOT_FOUND;
}

void Zone::turnZoneEnteredOff()
{
  for (auto creature : slots_)
  {
    if (creature != nullptr)
      creature->setEnter(false);
  }
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
