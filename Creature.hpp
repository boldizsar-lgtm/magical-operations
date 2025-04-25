#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <vector>
#include <string>
#include "Card.hpp"
#include "Trait.hpp"

class Creature : public Card
{
protected:
  int base_attack_;
  int base_health_;
  std::vector<Trait*> base_traits_;

  int current_attack_;
  int current_health_;
  std::vector<Trait*> current_traits_;

  std::vector<Trait*> preBattleTraits;
  std::vector<Trait*> midBattleTraits;
  std::vector<Trait*> postBattleTraits;
  
public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Creature() = default;
  Creature(Creature&) = default;
  ~Creature() = default;

  //Methods
  void setCurrentAttack(int new_attack);
  void setCurrentHealth(int new_health);
  void addTrait(Trait* trait);
  std::string getCardString(int row) const override;
};

#endif //CREATURE_HPP
