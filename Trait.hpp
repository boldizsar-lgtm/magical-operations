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
  char id_;
  std::string name_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Trait(std::string name, char id) : name_{name}, id_{id} {}
  Trait(Trait&) = default;
  ~Trait() = default;

  //Methods
  char getId() const;
  TraitType getType() const;
};

#endif //TRAIT_HPP
