#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>
#include "Card.hpp"

enum class SpellType
{
  GENERAL,
  TARGET,
  GRAVEYARD
};

class Spell : public Card
{
protected:
  SpellType subtype_;
  std::string effect_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Spell() = default;
  Spell(Spell&) = default;
  ~Spell() = default;

  //Method
  std::string getCardString(int row) const override;
};

#endif //SPELL_HPP
