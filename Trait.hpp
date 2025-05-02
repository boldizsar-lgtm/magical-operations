#ifndef TRAIT_HPP
#define TRAIT_HPP

#include <string>

/*enum class TraitType
{
  PRE_BATTLE,
  MID_BATTLE,
  POST_BATTLE,
  SPECIAL
};*/

class Trait
{
private:
  //TraitType type_;
  char id_;
  std::string name_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Trait(char id, std::string name) : id_{id}, name_{name} {}
  Trait(Trait&) = default;
  ~Trait() = default;

  //Methods
  char getID() const;
  TraitType getType() const;
};

#endif //TRAIT_HPP
