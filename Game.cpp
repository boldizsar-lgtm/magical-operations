#include "Game.hpp"

Game::Game(int max_rounds) : max_rounds_{max_rounds}, round_count_{0}, board_active_{true}, player1_{1}, player2_{2},
                             attacker_(&player1_), defender_(&player2_)
{
  //set default roles of players
  player1_.setRole(Role::ATTACKER);
  player2_.setRole(Role::DEFENDER);

  //print game start
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;
  std::cout << Config::getInstance().getMessage("D_WELCOME") << std::endl;
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;

}


bool Game::startRound()
{
  //end game if round_count reach max rounds
  if (round_count_++ == max_rounds_)
  {
    return false;
  }

  //switch attacking and defending player
  if (round_count_ % 2 == 0)
  {
    if (attacker_ == &player1_)
    {
      attacker_ = &player2_;
      defender_ = &player1_;
    }
    else
    {
      attacker_ = &player1_;
      defender_ = &player2_;
    }

    player1_.switchRole();
    player2_.switchRole();
  }

  //print round start
  std::cout << std::endl << Config::getInstance().getMessage("D_BORDER_D") << std::endl;
  std::cout << "                                         ROUND " << getRound() << std::endl;
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;

  //increase mana_pool by 1 and refill it
  if (round_count_ % 2 == 1)
  {
    player1_.increaseManaPool(1);
    player1_.refillCurrentMana();
    player2_.increaseManaPool(1);
    player2_.refillCurrentMana();
  }

  //print board
  printBoard();

  //players draw a card
  if (!attacker_->drawCard())
    return false;
  if (!defender_->drawCard())
    return false;

  return true;
}


bool Game::startSetupPhase()
{
  player1_.setDoneStatus(false);
  player2_.setDoneStatus(false);

  current_player_ = attacker_;

  CommandLine cli;
  // Command loop for setup phase from main to here, maybe combine in own function to avoid repetition
  if (doCommandLoop(cli) == false)
    return false;

  //switch current player
  current_player_ = defender_;

  //setup phase for opposing player
  if (doCommandLoop(cli) == false)
    return false;


  //TODO: check traits?

  return true;
}

bool Game::doCommandLoop(CommandLine& cli)
{
  while (!current_player_->getDoneStatus())
  {
    current_player_->printPlayerPrompt();
    Command command = cli.readCommand();
    if (command.isQuit())
      return false;
    processCommand(command);
  }
}

bool Game::startBattlePhase()
{
  //TODO
  return true;
}


void Game::processCommand(Command command)
{
  CommandType command_type = command.getType();
  int param_amount = command.getParameters().size();

  if (command_type == CommandType::HELP)
    printHelp();

  if (command_type == CommandType::GRAVEYARD)
    current_player_->printGraveyard();

  if (command_type == CommandType::BOARD)
    changeBoardStatus();

  if (command_type == CommandType::HAND)
    current_player_->printHand();

  if (command_type == CommandType::INFO)
    printInfo(command);

  if (command_type == CommandType::REDRAW)
  {
    //redraw current player's hand with 1 less card, disable this command if DONE, BATTLE, CREATURE, or SPELL has been used once
    // keine ahnung wie wir checken wollen welche commands schon benutzt wurden, entweder member variable vector wo benutzte hinzugefügt werden oder irgendwie in der main? - LENON
    // anscheinend hat die Player Class auch einen redraw command, maybe dadurch? - LENON
  }

  if (command_type == CommandType::STATUS)
    printStatus();

  if (command_type == CommandType::DONE)
  {
    //player ends their turn, the board is printed,
    current_player_->setDoneStatus(true);
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

int Game::getRound() const
{
  return round_count_;
}

void Game::printBoard() const
{
  if (board_active_ == true)
  {
    /*Syntax:
        Defender Border
        Defender Field Zone
        Border A
        Defender Battle Zone
        Border B
        Attacker Battle Zone
        Border A
        Attacker Field Zone
        Attacker Border*/
    std::cout << "================================== DEFENDER: PLAYER " << defender_->getId() <<
                 " ===================================" << std::endl;
    defender_->printFieldZone();
    std::cout << Config::getInstance().getMessage("D_BORDER_A") << std::endl;
    defender_->printBattleZone();
    std::cout << Config::getInstance().getMessage("D_BORDER_B") << std::endl;
    attacker_->printBattleZone();
    std::cout << Config::getInstance().getMessage("D_BORDER_A") << std::endl;
    attacker_->printFieldZone();
    std::cout << "================================== ATTACKER: PLAYER " << attacker_->getId() <<
                 " ===================================" << std::endl;
  }
}


// bool Game::checkEndConditions()
// {
//   /* 3 end conditions:
//     1. Health of one of the player below or equal to zero (during battle phase).
//     2. A Player needs to draw a card but there are no cards left in his deck (this player loses) (during setup phase).
//     3. Game max rounds is reached (pre setup phase)
//       - Player with highest health points wins
//       - Same health points -> tie!
//   */

//   //TODO: maybe check this with the Role enum
//   if (player1_.getRoleString() == "Defender")
//   {
//     if (player1_.getCurrentHealth() <= 0)
//     {
//       // TODO: winner is player2
//       continue_game_ = false;
//     }
//   }
//   else if (player2_.getRoleString() == "Defender")
//   {
//     if(player2_.getCurrentHealth() <= 0)
//     {
//       //TODO : winner is player1
//       continue_game_ = false;
//     }
//   }

//   if (current_player_->getDeckSize() == 0)
//   {
//     // TODO: current player loses
//     continue_game_ = false;
//   }

//   if (round_count_ == max_rounds_)
//   {
//     // TODO: determine winner
//     continue_game_ = false;
//   }
// }


void Game::printHelp() const
{
  std::cout << "=== Commands ============================================================================\n";
  std::cout << "- help\n    Prints this help text.\n\n- quit\n    Terminates the game.\n\n";
  std::cout << "- battle <FIELD_SLOT> <BATTLE_SLOT>\n";
  std::cout << "    Moves a creature from a Field Zone slot into a Battle Zone slot.\n";
  std::cout << "    <FIELD_SLOT>: Current slot of the creature (F1, ..., F7)\n";
  std::cout << "    <BATTLE_SLOT>: Battle slot for the creature (B1, ..., B7)\n\n- board\n";
  std::cout << "    Toggles the board printing.\n\n- creature <HAND_CARD_ID> <FIELD_SLOT>\n";
  std::cout << "    Places a creature from your hand into a Field Zone slot.\n";
  std::cout << "    <HAND_CARD_ID>: The ID of the creature card in your hand\n";
  std::cout << "    <FIELD_SLOT>: The Field Zone slot to place the creature in\n\n- done\n";
  std::cout << "    Ends your turn for this round.\n\n- graveyard\n    Prints all the cards in your graveyard.\n\n";
  std::cout << "- hand\n    Prints your hand cards.\n\n- info <CARD_ID>\n    Prints card information.\n";
  std::cout << "    <CARD_ID>: The ID of the card to be inspected\n\n- redraw\n";
  std::cout << "    Discards all hand cards and draws the same amount minus one from your deck.\n\n";
  std::cout << "- spell <HAND_CARD_ID> [<TARGET_SLOT>|<GRAVEYARD_CARD_ID>]\n    Casts a spell from your hand.\n";
  std::cout << "    <HAND_CARD_ID>: The ID of the spell in your hand\n";
  std::cout << "    <TARGET_SLOT>: The slot to target with a target spell\n";
  std::cout << "    <GRAVEYARD_CARD_ID>: The ID of a card in the graveyard to cast a graveyard spell on\n\n";
  std::cout << "- status\n    Prints general information about both players.\n\n";
  std::cout << "=========================================================================================\n";

}

void Game::printInfo(Command& command) const
{
  // Check if card exists:
  std::string card_id = command.getParameters().at(0);

  Card* card = Config::getInstance().getCard(card_id);

  // If it doesn't, throw an error
  if (card == nullptr)
    throw CommandError("E_INVALID_CARD");

  // Print info border
  std::cout << Config::getInstance().getMessage("D_BORDER_INFO") << std::endl;

  // Print Card info
  std::cout << card->getName() << " [" << card->getId() << "] (" << card->getManaCost() << " mana)" << std::endl;
  std::cout << "Type: " << card->getType() << std::endl;

  // Print card specifics
  if (card->getType() == "Creature")
  {
    // Cast a creature and print everything
    // Traits are handled in Creature Class
    Creature* creature = static_cast<Creature*>(card);
    std::cout << "Base Attack: " << creature->getBaseAttack() << std::endl;
    std::cout << "Base Health: " << creature->getBaseHealth() << std::endl;
    std::cout << "Base Traits: ";
    creature->printBaseTraitList();
  }
  else
  {
    // Cast a spell (ba-dum-tss) and print everything
    Spell* spell = static_cast<Spell*>(card);
    std::cout << "Effect: " << spell->getEffect() << std::endl;
  }

  // Print end border
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;
}

void Game::printStatus() const
{
  // Print info border
  std::cout << Config::getInstance().getMessage("D_BORDER_STATUS") << std::endl;

  // Print Player 1 info
  std::cout << "Player 1" << std::endl;
  std::cout << "Role: " << player1_.getRoleString() << std::endl;
  std::cout << "Health: " << player1_.getCurrentHealth() << std::endl;
  std::cout << "Mana: " << player1_.getCurrentMana() << " / " << player1_.getManaPool() << std::endl;
  std::cout << "Remaining Deck: " << player1_.getDeck()->getSize() << " card(s)" << std::endl;
  std::cout << "Graveyard Size: " << player1_.getGraveyard().getSize() << " card(s)" << std::endl;

   std::cout << Config::getInstance().getMessage("D_BORDER_C") << std::endl;

  // Print Player 2 info
  std::cout << "Player 2" << std::endl;
  std::cout << "Role: " << player2_.getRoleString() << std::endl;
  std::cout << "Health: " << player2_.getCurrentHealth() << std::endl;
  std::cout << "Mana: " << player2_.getCurrentMana() << " / " << player2_.getManaPool() << std::endl;
  std::cout << "Remaining Deck: " << player2_.getDeck()->getSize() << " card(s)" << std::endl;
  std::cout << "Graveyard Size: " << player2_.getGraveyard().getSize() << " card(s)" << std::endl;

  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;
}

void Game::changeBoardStatus()
{
  if (board_active_ == true)
    board_active_ = false;
  else if (board_active_ == false)
  {
    board_active_ = true;
    printBoard();
  }
}
