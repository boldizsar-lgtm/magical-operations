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

  // Methods
  virtual void printCardLine(int row) const = 0;
  virtual std::string getType() const = 0;
  std::string getId() const;
  std::string getName() const;
  int getManaCost() const;
  std::string getManaCostString() const;
};

#endif //CARD_HPP
