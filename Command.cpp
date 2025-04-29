#include "Command.hpp"
#include "Utils.hpp"

//Implemented Bobo's a1 version of command handling with adjustments for now
Command::Command(std::vector<std::string>& input)
{
  std::string command_string = input.at(0);
  Utils::toLower(command_string);

  if (command_string == "quit")
  {
    if (input.size() == 1)
      type_ = CommandType::QUIT;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "help")
  {
    if (input.size() == 1)
      type_ = CommandType::HELP;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "graveyard")
  {
    if (input.size() == 1)
      type_ = CommandType::GRAVEYARD;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "board")
  {
    if (input.size() == 1)
      type_ = CommandType::BOARD;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "hand")
  {
    if (input.size() == 1)
      type_ = CommandType::HAND;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "info")
  {
    if (input.size() == 2)
      type_ = CommandType::INFO;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "redraw")
  {
    if (input.size() == 1)
      type_ = CommandType::REDRAW;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "status")
  {
    if (input.size() == 1)
      type_ = CommandType::STATUS;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "done")
  {
    if (input.size() == 1)
      type_ = CommandType::DONE;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "battle")
  {
    if (input.size() == 3)
      type_ = CommandType::BATTLE;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "creature")
  {
    if (input.size() == 3)
      type_ = CommandType::CREATURE;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else if (command_string == "spell")
  {
    if (input.size() == 2 || input.size() == 3)
      type_ = CommandType::SPELL;
    else
      type_ = CommandType::WRONG_PARAM;
  }
  else
    type_ = CommandType::INVALID;
  
  parameters_ = std::vector<std::string>(input.begin() + 1, input.end());
}

bool Command::isQuit()
{
  if (type_ == CommandType::QUIT)
    return true;
  else
    return false;
}

CommandType Command::getType()
{
  return type_;
}

std::vector<std::string>& Command::getParameters()
{
  return parameters_;
}

void Command::setType(CommandType type)
{
  type_ = type;
}
