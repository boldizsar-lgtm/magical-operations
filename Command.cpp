#include "Command.hpp"
#include "Utils.hpp"

//Implemented Bobo's a1 version of command handling with adjustments for now
Command::Command(const std::vector<std::string>& input)
{
  std::string command_string = input.at(0);
  Utils::toUpper(command_string);

  if (input.size() == 1)
  {
    if (command_string == "QUIT")
      type_ = CommandType::QUIT;
    else if (command_string == "HELP")
      type_ = CommandType::HELP;
    else if (command_string == "GRAVEYARD")
      type_ = CommandType::GRAVEYARD;
    else if (command_string == "BOARD")
      type_ = CommandType::BOARD;
    else if (command_string == "HAND")
      type_ = CommandType::HAND;
    else if (command_string == "REDRAW")
      type_ = CommandType::REDRAW;
    else if (command_string == "STATUS")
      type_ = CommandType::STATUS;
    else if (command_string == "DONE")
      type_ = CommandType::DONE;
  }
  else if (input.size() == 2)
  {
    if (command_string == "INFO")
      type_ = CommandType::INFO;
  }
  else if (input.size() == 3)
  {
    if (command_string == "BATTLE")
      type_ = CommandType::BATTLE;
    else if (command_string == "CREATURE")
      type_ = CommandType::CREATURE;
  }
  else if (command_string == "SPELL")
    type_ = CommandType::SPELL;
  else
    type_ = CommandType::WRONG_PARAM;

  if (command_string != "QUIT" && command_string != "STATUS" && command_string != "GRAVEYARD" &&
      command_string != "HAND" && command_string != "BOARD"  && command_string != "CREATURE"  && 
      command_string != "DONE" && command_string != "INFO"   && command_string != "BATTLE"    &&
      command_string != "HELP" && command_string != "REDRAW" && command_string != "SPELL") 
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
