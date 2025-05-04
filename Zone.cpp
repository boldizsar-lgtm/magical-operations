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
  slots_[column] = creature;
}

void Zone::printZone(int format) const
{

}
