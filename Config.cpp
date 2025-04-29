#include <fstream>
#include "Config.hpp"

bool Config::readGameConfig(std::string path)
{
  //OPEN FILE
  std::ifstream file(path);
  if (!file.is_open())
    return false;
  
  //CHECK MAGIC NUMBER
  std::string magic_num;
  std::getline(file, magic_num);
  if (magic_num != "GAME")
    return false;

  //read in game configs
  return true;
}

bool Config::readMessageConfig(std::string path)
{
  //OPEN FILE
  std::ifstream file(path);
  if (!file.is_open())
    return false;
  
  //CHECK MAGIC NUMBER
  std::string magic_num;
  std::getline(file, magic_num);
  if (magic_num != "MESSAGE")
    return false;
  
  //read in messages
  return true;
}