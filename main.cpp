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
#include "CommandError.hpp"

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

  try
  {
    Config& config = Config::getInstance();
    if (!config.saveTraits("data/traits_config.txt"))
    {
      std::cout << "[ERROR] Not enough memory!\n" << std::endl;
      return NOT_ENOUGH_MEMORY;
    }
    config.saveCards("data/cards_config.txt");

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

    Game game(config.getMaxRounds());

    bool continue_game = true;
    while (continue_game)
    {
      try
      {
        continue_game = game.startRound();
        if (!continue_game)
          break;

        continue_game = game.startSetupPhase();
        if (!continue_game)
          break;

        continue_game = game.startBattlePhase();
        if (!continue_game)
          break;
      }
      catch(CommandError& e)
      {
        //TODO: do we need this here?
        std::cout << e.what() << std::endl;
      }
    }

    //start final phase and check if written to file
    if (!game.startFinalPhase(argv[1]))
    {
      std::cout << Config::getInstance().getMessage("I_FILE_WRITE_FAILED") << std::endl;
    }
  }
  catch(std::bad_alloc& e)
  {
    std::cout << "[ERROR] Not enough memory!\n" << std::endl;
    return NOT_ENOUGH_MEMORY;
  }

  return SUCCESSFUL;
}
