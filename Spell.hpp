#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>
#include "Card.hpp"

/*enum class SpellType
{
  GENERAL,
  TARGET,
  GRAVEYARD
};*/

class Spell : public Card
{
protected:
  //SpellType subtype_;
  std::string effect_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Spell(std::string id, int mana_cost, std::string name, std::string effect) : Card(id, mana_cost, name), effect_(effect) {}
  Spell(Spell&) = default;
  ~Spell() = default;

  //Method
  std::string getType() const override;
  std::string getCardString(int row) const override;
};

#endif //SPELL_HPP
