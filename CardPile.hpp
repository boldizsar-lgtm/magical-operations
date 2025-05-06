#ifndef CARDPILE_HPP
#define CARDPILE_HPP

#include <vector>
#include "Card.hpp"

enum class PileType
{
  HAND,
  DECK,
  GRAVEYARD
};

class CardPile
{
protected:
  PileType type_;
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
  Card* searchCard(std::string id) const;
  void removeTopCard();
  void removeEndCard();
  void printCardPile() const;
  void printGraveyard() const;
  Card* getPileEnd() const;
  Card* getPileTop() const;
  int getSize() const;
};

#endif //CARDPILE_HPP
