//---------------------------------------------------------------------------------------------------------------------
// Spell.hpp/Spell.cpp
//
// The Spell class contains getters for spell information and a printing function.
//
// Group: 026
//
// Author: Boldizsar Kiss, 12417350,
//         Lenon Osmani, 12303674,
//         Katharina Hartmann, 12219843
//---------------------------------------------------------------------------------------------------------------------
//

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


  //------------------------------------------------------------------------------------------------------------------
  // prints one of the four lines required to display a single card.
  //
  // @param row - an integer between 1 and 4, dictating which of the four lines should be printed
  //
  void printCardLine(int row) const override;

  //------------------------------------------------------------------------------------------------------------------
  // returns the type of the card as a string
  //
  // @return "Spell"
  //
  std::string getType() const override;

  //------------------------------------------------------------------------------------------------------------------
  // returns the subtype of the spell as an enum class SpellType
  //
  // @return GENERAL, TARGET, or GRAVEYARD
  //
  SpellType getSubtype() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the effect of the spell
  //
  // @return "All of the player's own creatures on the board gain the Haste and Temporary traits, and their current
  //          attack is increased by 3.", if used on the Battle Cry spell.
  //
  std::string getEffect() const;
};

#endif //SPELL_HPP
