#include <fstream>
#include "Config.hpp"

//read in the traits config file we created
//
//param: path, the path of the traits config file
//
//return: false if memory allocation error occurs, true if successful
bool saveTraits(std::string path)
{
  //open file
  std::ifstream file(path);
  
  //save traits
  //UNFINISHED

  return true;
}

//read in the cards config file we created
//
//param: path, the path of the cards config file
//
//return: false if memory allocation error occurs, true if successful
bool saveCards(std::string path)
{
  //open file
  std::ifstream file(path);
  
  //save cards
  //syntax: <ID>;<Mana Cost>;<Name>;<Base Traits>;<Base Attack>;<Base Health>
  std::string id;
  std::string str_mana_cost;
  std::string name;
  std::string base_traits;
  std::string str_base_attack;
  std::string str_base_health;
  int mana_cost;
  int base_attack;
  int base_health;
  while (std::getline(file, id, ';') &&
         std::getline(file, mana_cost, ';') &&
         std::getline(file, name, ';') &&
         std::getline(file, base_traits, ';') &&
         std::getline(file, base_attack, ';') &&
         std::getline(file, base_health, '\n'))
  {
    Utils::decimalStringToInt(str_mana_cost, mana_cost)
    Utils::decimalStringToInt(str_base_attack, base_attack)
    Utils::decimalStringToInt(str_base_health, base_health)
    //UNFINISHED
  }


  return true;
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
  Utils::decimalStringToInt(line, player_health_)
  std::getline(file, line);
  Utils::decimalStringToInt(line, max_rounds_)
  std::getline(file, line);
  Utils::decimalStringToInt(line, deck_size_)
  std::getline(file, line);
  Utils::decimalStringToInt(line, mana_pool_size)

  //save player decks
  //UNFINISHED

  return true;
}

//Open and read in the messages config file
//
//param: path, the path of the message config file
//
//return: false if file couldn't be opened or wrong Magic Number, true if successful
bool Config::readMessageConfig(std::string path)
{
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
  //UNFINISHED

  return true;
}


