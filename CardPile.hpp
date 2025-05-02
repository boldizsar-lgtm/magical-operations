#ifndef CARDPILE_HPP
#define CARDPILE_HPP

#include <vector>
#include "Card.hpp"
#include "Config.hpp"

class CardPile
{
protected:
  std::vector<Card*> pile_;
  
public:
  //Constructor, Copy-Assignment-Operator, Destructor
  CardPile() = default;
  CardPile(CardPile&) = default;
  ~CardPile() = default;

  //Methods
  void addCard(std::string id);
  void removeCard(int id);
  void removeTopCard();
  const std::string getCardPileString();
};

#endif //CARDPILE_HPP
