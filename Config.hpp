#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <memory>
#include <string>
#include "Card.hpp"
#include "Creature.hpp"
#include "Spell.hpp"
#include "Trait.hpp"
#include "Utils.hpp"
#include "CardPile.hpp"

//Singleton Class
class Config
{
private:
  int player_health_;
  int max_rounds_;
  int deck_size_;
  int mana_pool_size_;
  CardPile player_1_deck_;
  CardPile player_2_deck_;
  std::map<char, std::shared_ptr<Trait>> traits_;
  std::map<std::string, Card*> cards_;
  std::map<std::string, std::string> messages_;


  //Singleton Requirement
  static Config* instance_;

  /*Constructor, Copy-Assignment-Operator, Destructor,
  and Singleton Requirements*/
  Config() = default;
  Config(Config&) = delete;
  Config& operator=(const Config&) = delete;
  ~Config();


public:
  //Methods
  static Config& getInstance(); //Singleton Requirement
  bool saveTraits(std::string path);
  void saveCards(std::string path);
  bool readGameConfig(std::string path);
  bool readMessageConfig(std::string path);
  Card* getCard(std::string id);
  std::string getMessage(std::string key);
  int getMaxRounds() const;
  int getBaseHealth() const;
  int getBaseMana() const;
  CardPile getDeck(int player_id);
};

#endif //CONFIG_HPP
