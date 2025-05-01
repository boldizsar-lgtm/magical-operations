#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <string>
#include "Card.hpp"
#include "Trait.hpp"
#include "Utils.hpp"

class Config
{
protected:
  int player_health_;
  int max_rounds_;
  int deck_size_;
  int mana_pool_size_;
  std::map<char, Trait> traits_;
  std::map<std::string, Card> cards_;
  std::map<std::string, std::string> messages_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Config() = default;
  Config(Config&) = default;
  ~Config() = default;

  //Methods
  bool saveTraits(std::string path);
  bool saveCards(std::string path);
  bool readGameConfig(std::string path);
  bool readMessageConfig(std::string path);

};

#endif //CONFIG_HPP
