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


  //------------------------------------------------------------------------------------------------------------------
  // check if the command is QUIT
  //
  // @return true if the command is QUIT
  //         false if it isn't
  //
  bool isQuit();

  //------------------------------------------------------------------------------------------------------------------
  // return the command type
  //
  // @return enum class CommandType: QUIT, HELP, GRAVEYARD, BOARD, HAND, INFO, REDRAW, STATUS, DONE, BATTLE, CREATURE,
  //                                 SPELL,
  //                                 INVALID, if its none of the above
  //                                 WRONG_PARAM, if the parameter count is wrong for one of the above commands
  //
  CommandType getType();

  //------------------------------------------------------------------------------------------------------------------
  // return the parameters of the command
  //
  // @return a vector of strings, eaach string being one parameter
  //
  std::vector<std::string>& getParameters();

  //------------------------------------------------------------------------------------------------------------------
  // set the type of the command
  //
  void setType(CommandType type);

//End of copying
};

#endif
