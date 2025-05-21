//---------------------------------------------------------------------------------------------------------------------
// Trait.hpp/Trait.cpp
//
// The Trait class contains getters for the trait information.
//
// Group: 026
//
// Author: Boldizsar Kiss, 12417350,
//         Lenon Osmani, 12303674,
//         Katharina Hartmann, 12219843
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef TRAIT_HPP
#define TRAIT_HPP

#include <string>

class Trait
{
private:
  char id_;
  std::string name_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Trait(std::string name, char id) : id_{id}, name_{name} {}
  Trait(Trait&) = default;
  ~Trait() = default;

  
  //------------------------------------------------------------------------------------------------------------------
  // returns the one letter id of the trait
  //
  // @return 'H', if used on Haste
  //
  char getId() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns the name of the trait
  //
  // @return "Haste", if used on the trait with the id H
  //
  std::string getName() const;
};

#endif //TRAIT_HPP
