#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <memory>
#include <string>
#include "Card.hpp"
#include "Trait.hpp"
#include "Utils.hpp"
#include "CardPile.hpp"

class Config
{
protected:
  int player_health_;
  int max_rounds_;
  int deck_size_;
  int mana_pool_size_;
  CardPile player_1_deck_;
  CardPile player_2_deck_;
  std::map<char, std::shared_ptr<Trait>> traits_;
  std::map<std::string, Card> cards_;
  std::map<std::string, std::string> messages_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Config() = default;
  Config(Config&) = default;
  ~Config() = default;

  //Methods
  bool saveTraits(std::string path);
  void saveCards(std::string path);
  bool readGameConfig(std::string path);
  bool readMessageConfig(std::string path);
  void createGame();
};

#endif //CONFIG_HPP
