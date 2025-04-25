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
    Zone() : slots_(7, nullptr) {}
    Zone(Zone&) = default;
    ~Zone() = default;

    // Methods
    void removeCreature();  //both methods could work with (Creature* creature) or (std::string id) as parameters
    void addCreature();
    std::string printZone(int format) const;
};

#endif //ZOO_HPP
