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

  //------------------------------------------------------------------------------------------------------------------
  // create a deep copy of a card stored in the cards map in config, add add it to the bottom/end of the pile
  //
  // @param id, the id/key of the card that should be copied from thhe map
  //
  void createCard(std::string id);

  //------------------------------------------------------------------------------------------------------------------
  // add a card to the bottom/end of the pile
  //
  // @param card, pointer to the card that should be addded
  //
  void addCardEnd(Card* card);

  //------------------------------------------------------------------------------------------------------------------
  // add a card to the top/beginning of the pile
  //
  // @param card, pointer to the card that should be addded
  //
  void addCardTop(Card* card);

  //------------------------------------------------------------------------------------------------------------------
  // remove the first card with a given id from the pile
  //
  // @param id, the id of the card that should be removed
  //
  void removeCard(std::string id);

  //------------------------------------------------------------------------------------------------------------------
  // search the pile for a card with a given id
  //
  // @param id, the id of the card that should found
  //
  // @return a pointer to the card
  //         a nullptr, if no card with the given id is in the pile
  //
  Card* searchCard(std::string id) const;

  //------------------------------------------------------------------------------------------------------------------
  // return the pile of cards
  //
  // @return a vector of card pointers
  //
  std::vector<Card*> getPile() const;

  //------------------------------------------------------------------------------------------------------------------
  // remove the top/beginning card of the pile
  //
  void removeTopCard();

  //------------------------------------------------------------------------------------------------------------------
  // remove the bottom/end card of the pile
  //
  void removeEndCard();

  //------------------------------------------------------------------------------------------------------------------
  // prints the entire card pile with mana cost, id, type, etc for the Hand command
  //
  void printCardPile() const;

  //------------------------------------------------------------------------------------------------------------------
  // prints the entire card pile with name and id for the Graveyard command
  //
  void printGraveyard() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the bottom/end card of the pile
  //
  // @return a card pointer
  //
  Card* getPileEnd() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the top/beginning card of the pile
  //
  // @return a card pointer
  //
  Card* getPileTop() const;
  int getSize() const;
};

#endif //CARDPILE_HPP
