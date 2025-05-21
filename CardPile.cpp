#include <iostream>
#include "CardPile.hpp"
#include "Config.hpp"

const int ZONE_WIDTH (7);

void CardPile::createCard(std::string id)
{
  Card* card = Config::getInstance().getCard(id);
  Card* new_card;

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

    pile_.push_back(new_card);
  }
}

void CardPile::addCardEnd(Card* card)
{
  //adding and existing card to end of a pile
  pile_.push_back(card);
}

void CardPile::addCardTop(Card* card)
{
  //adding and existing card to top of the pile
  pile_.insert(pile_.begin(), card);
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
}

Card* CardPile::searchCard(std::string id) const
{
  for (auto& card : pile_)
  {
    if (card->getId() == id)
    {
      return card;
    }
  }
  return nullptr;
}

std::vector<Card*> CardPile::getPile() const
{
  return pile_;
}

void CardPile::removeEndCard()
{
  if (pile_.size() != 0)
    pile_.pop_back();
}

void CardPile::removeTopCard()
{
  if (pile_.size() != 0)
    pile_.erase(pile_.begin());
}

void CardPile::printCardPile() const
{
  float required_card_rows = (pile_.size() + 6) / ZONE_WIDTH;

  size_t current_card_rows = 0;

  while (current_card_rows < required_card_rows)
  {
    for (int print_row = 1; print_row <= 4; print_row++)
    {
      std::cout << ' ';
      for (size_t i = current_card_rows * ZONE_WIDTH; i < (current_card_rows + 1) * ZONE_WIDTH && i < pile_.size(); i++)
      {
        //print first 3 empty spaces
        std::cout << "   ";
        pile_[i]->printCardLine(print_row);
      }

      std::cout << std::endl;
    }

    current_card_rows++;
  }
}

void CardPile::printGraveyard() const
{
  if (pile_.size() > 0)
  {
    for (size_t i = 0; i < pile_.size(); i++)
    {
      std::cout << pile_[i]->getId() << " | " << pile_[i]->getName() << std::endl;
    }
  }
}

Card* CardPile::getPileEnd() const
{
  if (pile_.size() != 0)
    return pile_.back();
  else
  {
    return nullptr;
  }
}

Card* CardPile::getPileTop() const
{
  if (pile_.size() != 0)
    return pile_.front();
  else
  {
    return nullptr;
  }
}

int CardPile::getSize() const
{
  return pile_.size();
}

CardPile::~CardPile()
{
  for(auto& card : pile_)
  {
    if (card)
    {
      delete card;
    }
  }
}
