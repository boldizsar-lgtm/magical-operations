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


  //Singleton Requirement, this and other singleton requirements were implemented and explained with the help of
  //chatGPT and https://refactoring.guru/design-patterns/singleton/cpp/example
  static Config* instance_;

  /*Constructor, Copy-Assignment-Operator, Destructor,
  and Singleton Requirements*/
  Config() = default;
  Config(Config&) = delete;
  Config& operator=(const Config&) = delete;
  ~Config();

public:
  //------------------------------------------------------------------------------------------------------------------
  // returns a refrence to the only instance of config
  // Singleton Requirement
  //
  // @return Config refrence
  //
  static Config& getInstance();

  //------------------------------------------------------------------------------------------------------------------
  // reads in the traits config file we created and saves them in a map "traits" (save them as trait shared pointers)
  //
  // @param path, the path to the traits config file
  //
  // @return false, if memory allocation error occurs
  //         true, if successful
  //
  bool saveTraits(std::string path);

  //------------------------------------------------------------------------------------------------------------------
  // reads in the cards config file we created and saves them in a map "cards"
  //
  // @param path, the path to the cards config file
  //
  void saveCards(std::string path);

  //------------------------------------------------------------------------------------------------------------------
  // reads in the game config file and saves them in the config class' attributes
  //
  // @param path, the path to the game config file, entered as a command line parameter
  //
  // @return false, if the file couldn't be openend
  //         true, if successful
  //
  bool readGameConfig(std::string path);

  //------------------------------------------------------------------------------------------------------------------
  // reads in the message config file and saves them in a map "messages"
  //
  // @param path, the path to the message config file, entered as a command line parameter
  //
  // @return false, if the file couldn't be openend
  //         true, if successful
  //
  bool readMessageConfig(std::string path);

  //------------------------------------------------------------------------------------------------------------------
  // returns a shared pointer to a trait in the map saved under a specific id/key
  //
  // @param id, the id of the trait
  //
  // @return a shared pointer to the trait
  //         a nullptr if the trait doesnt exist
  //
  std::shared_ptr<Trait> getTrait(char id);

  //------------------------------------------------------------------------------------------------------------------
  // returns a pointer to a card in the map saved under a specific id/key
  //
  // @param id, the id of the card
  //
  // @return a pointer to the card
  //         a nullptr if the card doesnt exist
  //
  Card* getCard(std::string id);

  //------------------------------------------------------------------------------------------------------------------
  // returns a message string saved in the map under a specific key
  //
  // @param key, the key of the message
  //
  // @return the message string
  //         an empty string if the message doesnt exist
  //
  std::string getMessage(std::string key);

  //------------------------------------------------------------------------------------------------------------------
  // returns max_rounds_, read in through the game config file
  //
  // @return int, max_rounds_
  //
  int getMaxRounds() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns player's base_health_, read in through the game config file
  //
  // @return int, base_health_
  //
  int getBaseHealth() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns player's base_mana_, read in through the game config file
  //
  // @return int, base_mana_
  //
  int getBaseMana() const;

  //------------------------------------------------------------------------------------------------------------------
  // returns a pointer to a specific player's deck, which was created while reading in the game config file
  //
  // @param player_id, 1 returns player 1's deck
  //                   any other integeer (i.e 2) returns player 2's deck
  //
  // @return a CardPile pointer
  //
  CardPile* getDeck(int player_id);
};

#endif //CONFIG_HPP
