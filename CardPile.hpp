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
  ~CardPile();

  //Methods
  void createCard(std::string id);
  void addCardEnd(Card* card);
  void addCardTop(Card* card);
  void removeCard(std::string id);
  void removeTopCard();
  void printCardPile() const;
  Card* getPileEnd() const;
  int getSize() const;
};

#endif //CARDPILE_HPP
