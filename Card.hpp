#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card
{
protected:
  std::string name_;
  std::string id_;
  int mana_cost_;
  
public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Card(std::string name, std::string id, int mana_cost) : name_{name}, id_{id}, mana_cost_{mana_cost} {}
  Card(Card&) = default;
  virtual ~Card() = default;

  //Methods
  virtual std::string getCardString(int row) const = 0;
};

#endif //CARD_HPP
