#ifndef ZONE_HPP
#define ZONE_HPP

#include <vector>
#include <string>
#include "Creature.hpp"
#include "Config.hpp"

class Zone
{
private:
  std::vector<Creature*> slots_;

public:
  // Constructor, Copy-Assignment-Operator, Destructor
  Zone() : slots_(7, nullptr) {}
  Zone(Zone&) = default;
  ~Zone();

  //------------------------------------------------------------------------------------------------------------------
  // sets a specific column of the zone to a nullptr, removing the creature pointer if any was on it
  //
  // @param column, the column from which the creature is removed
  //
  void removeCreature(int column);

  //------------------------------------------------------------------------------------------------------------------
  // adds a creature to a specific column of the zone, set its stats to its base stats
  //
  // @param creature, a creature pointer
  // @param column, the column to which a creature is added
  //
  void addCreature(Creature* creature,int column);

  //------------------------------------------------------------------------------------------------------------------
  // adds a creature to a specific column of the zone, used to switch creatures from zone to zone without resetting
  // their stats 
  //
  // @param creature, a creature pointer
  // @param column, the column to which a creature is added
  //
  void switchCreaturePlace(Creature* creature, int column);

  //------------------------------------------------------------------------------------------------------------------
  // prints a row of the zone with empty fields and/or creature cards in it, required for the gameboard printing
  //
  // @param row, an integer between 1 and 4 dictating which of the rows that make up a zone print should be printed
  //
  void printZone(int row) const;

  //------------------------------------------------------------------------------------------------------------------
  // checks if a specific column of the zone is occupied by a creature
  //
  // @param column, the column which is checked
  //
  // @return true, if the column is occupied
  //         false, if the column is empty
  //
  bool isOccupied(int column) const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the creature that is occupying a specific column in the zone
  //
  // @param column, the column which is checked
  //
  // @return a pointer to the creature
  //
  Creature* getCreature(int column) const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the slots_ attribute
  //
  // @return a vector of creature pointers
  //
  std::vector<Creature*> getSlots() const;
  
  //------------------------------------------------------------------------------------------------------------------
  // returns the column number of the slot which a specific creature is occupying
  //
  // @param creature, a creature pointer that is being looked for
  //
  // @return an integer between 1 and 7 if the creature being looked for is found
  //         -1 if that creature is not in this zone
  //
  int searchCreature(Creature* creature);

  //------------------------------------------------------------------------------------------------------------------
  // make all creatures in the zone able to attack in the next combat, by turning their "entered_this_turn_"
  // attribute to false
  //
  void turnZoneEnteredOff();
};

#endif //ZONE_HPP
