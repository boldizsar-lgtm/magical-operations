#include <iostream>
#include "CardPile.hpp"
#include "Config.hpp"

void CardPile::createCard(std::string id)
{
  Card* card = Config::getInstance().getCard(id);
  Card* new_card;

  //TODO: throw exeption if allocation failed
  //alternatives for this code: implement copy constructor Card(const Card&) accordingly OR use std::unique_ptr

  //creating deep copy of a card object from the map and saving it to a card pile
  if(card != nullptr)
  {
    if (card->getType() == "Spell")
    {
        new_card = new Spell(*dynamic_cast<Spell*>(card));
    }
    else
    {
        new_card = new Creature(*dynamic_cast<Creature*>(card));
    }

    if (new_card)
      pile_.push_back(new_card);
    //TODO: else throw exeption
  }
}

void CardPile::addCard(Card* card)
{
  //adding card that already exists to a pile
  pile_.push_back(card);
}

void CardPile::removeCard(std::string id)
{
  //searching for card in a card pile
  int index{0};
  for (auto& card : pile_)
  {
    if (card->getId() == id)
    {
      //deleting card out of pile, without destroying the card object
      pile_.erase(pile_.begin() + index);
      return;
    }
    index++;
  }

  //TODO: throw error if card is not found?

}

void CardPile::removeTopCard()
{
  //TODO
}

const void CardPile::printCardPile()
{
  for (int row = 1; row <= 4; row++)
  {
    for (auto& c : pile_)
    {
      c->printCardLine(row);
      std::cout << " ";

      if (c == pile_.back())
        std::cout << std::endl;
    }
  }
}