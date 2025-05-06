#ifndef ZONE_HPP
#define ZONE_HPP

#include <vector>
#include <string>
#include "Creature.hpp"

enum class ZoneType
{
  FIELD,
  BATTLE
};

class Zone
{
private:
    ZoneType type_;
    std::vector<Creature*> slots_;

public:
    // Constructor, Copy-Assignment-Operator, Destructor
    Zone(ZoneType type) : type_{type}, slots_(7, nullptr) {}
    Zone(Zone&) = default;
    ~Zone();

    // Methods
    void removeCreature(std::string id);  //both methods could work with (Creature* creature) or (std::string id) as parameters
    void addCreature(Creature* creature,int column);
    void printZone(int format) const;
    bool isOccupied(int column);
};

#endif //ZOO_HPP
