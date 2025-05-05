#include "Trait.hpp"

char Trait::getId() const
{
  return id_;
}


TraitType Trait::getType() const
{
  return type_;
}


std::string Trait::getName() const
{
  return name_;
}
