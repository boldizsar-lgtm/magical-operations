#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>
#include <iostream>
#include "Card.hpp"

class Config;
class Game;

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
  Spell(std::string id, int mana_cost, std::string name, SpellType subtype, std::string effect) :
    Card(id, mana_cost, name), subtype_(subtype), effect_(effect) {}

  Spell(Spell&) = default;
  ~Spell() = default;

  //Method
  std::string getType() const override;
  void printCardLine(int row) const override;

  void execute(const Game& game);

  SpellType getSubtype() const;
  std::string getEffect() const;
};

#endif //SPELL_HPP
