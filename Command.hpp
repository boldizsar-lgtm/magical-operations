#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

enum class CommandType
{
  QUIT,
  HELP,
  GRAVEYARD,
  BOARD,
  HAND,
  INFO,
  REDRAW,
  STATUS,
  DONE,
  BATTLE,
  CREATURE,
  SPELL,
  INVALID,
  WRONG_PARAM
};

//Copied from OOP a1 (for now)
class Command
{
protected:
  CommandType type_;
  std::vector<std::string> parameters_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Command(const std::vector<std::string>& input);
  Command(CommandType type) : type_{type} {}
  Command(const Command&) = default;
  ~Command() = default;

  //Methods
  bool isQuit();
  CommandType getType();
  std::vector<std::string>& getParameters();
  void setType(CommandType type);
//End of copying
};

#endif
