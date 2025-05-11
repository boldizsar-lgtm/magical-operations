#ifndef ZONE_HPP
#define ZONE_HPP

#include <vector>
#include <string>
#include "Creature.hpp"
#include "Config.hpp"

enum class ZoneType
{
  FIELD,
  BATTLE
};

class Zone
{
private:
    ZoneType type_; //compiler warning that this isnt used, we could remove it
    std::vector<Creature*> slots_;

public:
    // Constructor, Copy-Assignment-Operator, Destructor
    Zone(ZoneType type) : type_{type}, slots_(7, nullptr) {}
    Zone(Zone&) = default;
    ~Zone();

    // Methods
    void removeCreature(int column);
    void addCreature(Creature* creature,int column);
    void switchCreaturePlace(Creature* creature, int column);
    void printZone(int format) const;
    bool isOccupied(int column) const;
    Creature* getCreature(int column) const;
    std::vector<Creature*> getSlots() const;
    
    
    int searchCreature(Creature* creature);
    void turnZoneEnteredOff();
};

#endif //ZONE_HPP
