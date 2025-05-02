//----------------------------------------------------------------------------------------------------------------------
// main.cpp
//
// Borrowed from a1 and adjusted
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include "Config.hpp"
#include "Game.hpp"
#include "Utils.hpp"
#include "CommandLine.hpp"
#include "Command.hpp"

enum ReturnValues
{
  SUCCESSFUL = 0,
  NOT_ENOUGH_MEMORY = 1,
  WRONG_NUMBER_OF_PARAMETERS = 2,
  INVALID_FILE = 3
};

//----------------------------------------------------------------------------------------------------------------------
// The main program. Controls the application on a high level view.
// Creates the game and a command line interface, handles the user input and execution of commands.
//
//
// @param argc, argument count, has to be exactly 2
// @param argv, argument vector, contains the game config file, and the message config file
// @return 0 (SUCCESSFUL), if the application was closed without errors
//         a non-zero number is returned in the case of failure
//
int main(int argc, char* argv[])
{
  //Syntax: /path/to/executable <GAME_CONFIG_FILE> <MESSAGE_CONFIG_FILE>
  
  Config config;
  if (!saveTraits("custom_configs/traits_config.txt"))
  {
    std::cout << "[ERROR] Not enough memory!\n" << std::endl;
    return NOT_ENOUGH_MEMORY;
  }
  saveCards("custom_configs/cards_config.txt")
  
  if (argc != 3)
  {
    std::cout << "[ERROR] Wrong number of parameters." << std::endl;
    return WRONG_NUMBER_OF_PARAMETERS;
  }

  if (!config.readGameConfig(argv[1]))
  {
    std::cout << "[ERROR] Invalid file " << argv[1] << '.' << std::endl;
    return INVALID_FILE;
  }

  if (!config.readMessageConfig(argv[2]))
  {
    std::cout << "[ERROR] Invalid file " << argv[2] << '.' << std::endl;
    return INVALID_FILE;
  }

  config.createGame();

  CommandLine cli;
  while (true)
  {
    Command command = cli.readCommand();
    if (command.isQuit())
      break;
    //Game needs to be created before this point
    game.processCommand(command);
  }

  return SUCCESSFUL;
}
