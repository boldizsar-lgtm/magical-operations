#include "CardPile.hpp"

void CardPile::addCard(std::string id)
{
  pile_.push_back(Config::getInstance().getCard(id));
}