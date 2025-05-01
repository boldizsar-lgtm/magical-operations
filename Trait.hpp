#ifndef TRAIT_HPP
#define TRAIT_HPP

#include <string>

enum class TraitType
{
  PRE_BATTLE,
  MID_BATTLE,
  POST_BATTLE,
  SPECIAL
};

class Trait
{
private:
  TraitType type_;
  std::string name_;
  char id_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Trait() = default;
  Trait(Trait&) = default;
  ~Trait() = default;

  //Methods
  char getID() const;
  TraitType getType() const;
};

#endif //TRAIT_HPP
