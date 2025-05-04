#include <fstream>
#include <iostream>
#include "Config.hpp"

//Singleton Requirements
Config& Config::getInstance()
{
  static Config instance;
  return instance;
}

//read in the traits config file we created
//
//param: path, the path of the traits config file
//
//return: false if memory allocation error occurs, true if successful
bool Config::saveTraits(std::string path)
{
  //open file
  std::ifstream file(path);
  
  //save traits
  std::string id_str;
  std::string trait;
  try {
    while (std::getline(file, id_str, ';') && std::getline(file, trait))
    {
      char id = id_str[0];
      traits_[id] = std::make_shared<Trait>(trait, id);
    }
  } catch (const std::bad_alloc& e)
  {
    return false;
  }
  
  return true;
}

//read in the cards config file we created
//
//param: path, the path of the cards config file
void Config::saveCards(std::string path)
{
  //open file
  std::ifstream file(path);

  //read in creatures
  //syntax: <ID>;<Mana Cost>;<Name>;<Base Traits>;<Base Attack>;<Base Health>
  std::string id;
  std::string mana_cost_str;
  std::string name;
  std::string base_traits_str;
  std::string base_attack_str;
  std::string base_health_str;
  while (std::getline(file, id, ';') &&
         (id != " ") &&
         std::getline(file, mana_cost_str, ';') &&
         std::getline(file, name, ';') &&
         std::getline(file, base_traits_str, ';') &&
         std::getline(file, base_attack_str, ';') &&
         std::getline(file, base_health_str))
  {
    int mana_cost = 0;
    int base_attack = 0;
    int base_health = 0;
    Utils::decimalStringToInt(mana_cost_str, mana_cost);
    Utils::decimalStringToInt(base_attack_str, base_attack);
    Utils::decimalStringToInt(base_health_str, base_health);

    //look through the map and create the base_traits vector
    std::vector<std::shared_ptr<Trait>> base_traits;
    for (char c : base_traits_str)
    {
      auto it = traits_.find(c);
      base_traits.push_back(it->second);
    }

    //create creature
    cards_[id] = new Creature(id, mana_cost, name, base_traits, base_attack, base_health);
  }

  //read in empty line
  std::string empty_line;
  std::getline(file, empty_line);

  //read in spells
  //syntax: <ID>;<Mana Cost>;<Name>;<Effect>
  std::string effect;
  while (std::getline(file, id, ';') &&
         std::getline(file, mana_cost_str, ';') &&
         std::getline(file, name, ';') &&
         std::getline(file, effect))
  {
    int mana_cost = 0;
    if (mana_cost_str == "x")
      mana_cost = -1; //MY THOUGHT WAS TO SAVE X COSTED SPELLS AS -1 AND DEAL WITH THEM IN THE SPELL CLASS
    else 
      Utils::decimalStringToInt(mana_cost_str, mana_cost);

    //create spell
    cards_[id] = new Spell(id, mana_cost, name, effect);
  }
}

//Open and read in the game config file
//
//param: path, the path of the game config file
//
//return: false if file couldn't be opened or wrong Magic Number, true if successful
bool Config::readGameConfig(std::string path)
{
  /*Syntax:
  <MAGIC_NUMBER>
  <PLAYER_HEALTH>
  <MAX_ROUNDS>
  <DECK_SIZE>
  <MANA_POOL_SIZE>
  /n
  Card_IDs of Player 1's Deck
  Card_IDs of Player 2's Deck*/

  //open file
  std::ifstream file(path);
  if (!file.is_open())
    return false;

  //check magic number
  std::string magic_num;
  std::getline(file, magic_num);
  if (magic_num != "GAME")
    return false;

  //save game configs
  std::string line;
  std::getline(file, line);
  Utils::decimalStringToInt(line, player_health_);
  std::getline(file, line);
  Utils::decimalStringToInt(line, max_rounds_);
  std::getline(file, line);
  Utils::decimalStringToInt(line, deck_size_);
  std::getline(file, line);
  Utils::decimalStringToInt(line, mana_pool_size_);

  //read in empty line
  std::string empty_line;
  std::getline(file, empty_line);

  //save player 1's deck
  for (int i = 1; i <= deck_size_; i++)
  {
    if (i < deck_size_)
      std::getline(file, line, ';');
    else
      std::getline(file, line, '\n');

    player_1_deck_.createCard(line);
  }

  //save player 2's deck
  for (int i = 1; i <= deck_size_; i++)
  {
    if (i < deck_size_)
      std::getline(file, line, ';');
    else
      std::getline(file, line, '\n');

    player_2_deck_.createCard(line);
  }

  player_1_deck_.printCardPile(); // TEMPORARY
  return true;
}

//Open and read in the messages config file
//
//param: path, the path of the message config file
//
//return: false if file couldn't be opened or wrong Magic Number, true if successful
bool Config::readMessageConfig(std::string path)
{
  //Syntax: <Key>:<Message>
  
  //open file
  std::ifstream file(path);
  if (!file.is_open())
    return false;
  
  //check magic number
  std::string magic_num;
  std::getline(file, magic_num);
  if (magic_num != "MESSAGE")
    return false;
  
  //save messages
  std::string key;
  std::string message;
  while (getline(file, key, ':') &&
         getline(file, message, '\n'))
  {
    if (key[0] == 'E')
      messages_[key] = "[ERROR] " + message;
    if (key[0] == 'I')
      messages_[key] = "[INFO] " + message;
    else
      messages_[key] = message;
  }

  return true;
}

Card* Config::getCard(std::string id)
{
  if (cards_.find(id) == cards_.end())
    return nullptr;
  else
    return cards_[id];
}

std::string Config::getMessage(std::string key)
{
  if (messages_.find(key) == messages_.end())
    return "";
  else
    return messages_[key];
}

int Config::getMaxRounds() const
{
  return max_rounds_;
}


int Config::getBaseHealth() const
{
  return player_health_;
}


int Config::getBaseMana() const
{
  return mana_pool_size_;
}


CardPile Config::getDeck(int player_id)
{
  if (player_id == 1)
  {
    return player_1_deck_;
  }
  else
  {
    return player_2_deck_;
  }
}



