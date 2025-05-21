//---------------------------------------------------------------------------------------------------------------------
// Card.hpp/Card.cpp
//
// The Card class contains all basic information of a single card and methods to access these values.
//
// Group: 026
//
// Author: Boldizsar Kiss, 12417350,
//         Lenon Osmani, 12303674,
//         Katharina Hartmann, 12219843
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card
{
protected:
  std::string id_;
  int mana_cost_;
  std::string name_;

public:
  // Constructor, Copy-Assignment-Operator, Destructor
  Card(std::string id, int mana_cost, std::string name) : id_{id}, mana_cost_{mana_cost}, name_{name} {}
  Card(Card&) = default;
  virtual ~Card() = default;


  //------------------------------------------------------------------------------------------------------------------
  // prints one of the four lines required to display a single card.
  //
  // @param row - an integer between 1 and 4, dictating which of the four lines should be printed
  //
  virtual void printCardLine(int row) const = 0;

  //------------------------------------------------------------------------------------------------------------------
  // returns the type of the card as a string
  //
  // @return "Creature", if its a creature card
  //         "Spell", if its a spell card
  //
  virtual std::string getType() const = 0;

  //------------------------------------------------------------------------------------------------------------------
  // returns the 5 letter id of the card as a string
  //
  // @return "ANGRT", if used on the Angry Rat card
  //
  std::string getId() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the name of the card as a string
  //
  // @return "Angry Rat", if used on the card with the id ANGRT
  //
  std::string getName() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the mana cost of the card
  //
  // @return -1, if the mana cost is x, or the normal integer if the mana cost is anything else
  //          (we saved x spells as -1 cost)
  //
  int getManaCost() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the the mana cost as a string, for the card prints
  //
  // @return "01", if the mana cost is 1
  //         "10", if the mana cost is 10
  //         "XX", if the mana cost is x
  //
  std::string getManaCostString() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the the mana cost as a string, for the info command
  //
  // @return "1", if the mana cost is 1
  //         "10", if the mana cost is 10
  //         "XX", if the mana cost is x
  //
  std::string getManaCostStringInfo() const;
};

#endif //CARD_HPP
