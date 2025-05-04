#ifndef CARDPILE_HPP
#define CARDPILE_HPP

#include <vector>
#include "Card.hpp"


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
  void createCard(std::string id);
  void addCard(Card* card);
  void removeCard(std::string id);
  void removeTopCard();
  const std::string getCardPileString();
};

#endif //CARDPILE_HPP
