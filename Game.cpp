#include "Game.hpp"

void Game::printBoard()
{

}

void Game::processCommand(Command command)
{
  CommandType command_type = command.getType();
  int param_amount = command.getParameters().size();

  if (command_type == CommandType::HELP)
  {
    //print all available commands
  }
  if (command_type == CommandType::GRAVEYARD)
  {
    //print current player's graveyard
  }
  if (command_type == CommandType::BOARD)
  {
    if (board_active_ == true)
      board_active_ = false;
    else if (board_active_ == false)
    {
      board_active_ = true;
      printBoard();
    }
  }
  if (command_type == CommandType::HAND)
  {
    //print current player's hand
  }
  if (command_type == CommandType::INFO)
  {
    //print info of the card entered as the parameter, check based on Card_ID. Check if its an existing ID
  }
  if (command_type == CommandType::REDRAW)
  {
    //redraw current player's hand with 1 less card, disable this command if DONE, BATTLE, CREATURE, or SPELL has been used once
  }
  if (command_type == CommandType::STATUS)
  {
    //print game status
  }
  if (command_type == CommandType::DONE)
  {
    //player ends their turn, the board is printed, 
    //check regenerate, and poisoned
  }
  if (command_type == CommandType::BATTLE)
  {
    //Syntax: battle <FIELD_SLOT> <BATTLE_SLOT>
    //move current player's creature from the field zone slot to the battle zone slot, a creature that was played this round cant battle, unless it has haste
    //check challenger, and haste
  }
  if (command_type == CommandType::CREATURE)
  {
    //Syntax: creature <HAND_CARD_ID> <FIELD_SLOT>
    //a creature is placed from the current player's hand onto the chosen field slot
  }
  if (command_type == CommandType::SPELL)
  {
    //Syntax: spell <HAND_CARD_ID> <OPTIONAL_ADDITIONAL_PARAMETER>
    //cast a spell from the current player's hand
  }
}