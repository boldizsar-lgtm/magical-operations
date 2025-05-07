#include "Game.hpp"
#include <fstream>

Game::Game(int max_rounds) : player1_{1}, player2_{2}, attacker_(&player1_), defender_(&player2_),
                             max_rounds_{max_rounds}, round_count_{0}, board_active_{true}
                             , winner_id_{0}
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
    end_condition_ = EndCondition::MAX_ROUNDS;
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
  {
    end_condition_ = EndCondition::DRAW;
    winner_id_ = defender_->getId();
    return false;
  }
  if (!defender_->drawCard())
  {
    end_condition_ = EndCondition::DRAW;
    winner_id_ = attacker_->getId();
    return false;
  }

  return true;
}


bool Game::startSetupPhase()
{
  player1_.setDoneStatus(false);
  player2_.setDoneStatus(false);

  current_player_ = attacker_;
  opposing_player_ = defender_;

  CommandLine cli;
  // Command loop for setup phase
  if (doCommandLoop(cli) == false)
    return false;

  //switch current player
  current_player_ = defender_;
  opposing_player_ = attacker_;

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
    try
    {
      current_player_->printPlayerPrompt();
      Command command = cli.readCommand();
      if (command.isQuit())
      {
        end_condition_ = EndCondition::QUIT;
        return false;
      }
      processCommand(command);
    }
    catch(const CommandError& e)
    {
      std::cout << e.what() << '\n';
    }
  }

  return true;
}

bool Game::startBattlePhase()
{
  std::cout << '\n' << Config::getInstance().getMessage("D_BORDER_BATTLE_PHASE") << '\n';

  for (int i = 1; i <= 7; i++)
  {
    std::cout << "---------------------------------------- SLOT " << i;
    std::cout << " -----------------------------------------\n";
    if (attacker_->getZone(ZoneType::BATTLE).isOccupied(i))
    {
      if (!startBattle(i))
        return false;
    }
  }

  std::cout << Config::getInstance().getMessage("D_BORDER_BATTLE_END") << '\n';

  return true;
}


bool Game::startBattle(int column)
{
  int amount{0};

  //Direct hit battle
  if (!defender_->getZone(ZoneType::BATTLE).isOccupied(column))
  {
    amount = defender_->getCurrentHealth() - attacker_->getZone(ZoneType::BATTLE).getCreature(column)->getAttack();

    //end condition DEFEAT reached
    if (amount <= 0)
    {
      end_condition_ = EndCondition::DEFEAT;
      winner_id_ = attacker_->getId();
      return false;
    }

    //reduce defenders health
    defender_->changeCurrentHealth((-attacker_->getZone(ZoneType::BATTLE).getCreature(column)->getAttack()));
    std::cout << Config::getInstance().getMessage("I_DIRECT");
    return true;
  }

  //Fight between two creatures
  std::cout << Config::getInstance().getMessage("I_FIGHT");

  //TODO: check traits and fight



  return true;
}


void Game::processCommand(Command command)
{
  CommandType command_type = command.getType();

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
    processRedraw(command);
  }

  if (command_type == CommandType::STATUS)
    printStatus();

  if (command_type == CommandType::DONE)
  {
    //player ends their turn, the board is printed,
    current_player_->setDoneStatus(true);
    printBoard();
    //check regenerate, and poisoned

    current_player_->setRedrawNotPossible();
  }

  if (command_type == CommandType::BATTLE)
  {
    processBattle(command);
    
    current_player_->setRedrawNotPossible();
  }

  if (command_type == CommandType::CREATURE)
  {
    processCreature(command);
    current_player_->setRedrawNotPossible();
  }
    

  if (command_type == CommandType::SPELL)
  {
    processSpell(command);
    current_player_->setRedrawNotPossible();
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


bool Game::startFinalPhase(std::string file_path)
{
  if (end_condition_ != EndCondition::QUIT)
    std::cout << '\n' << Config::getInstance().getMessage("D_BORDER_GAME_END") << std::endl;

  //print game end message
  switch (end_condition_)
  {
  case EndCondition::QUIT:
    return true;
  break;

  case EndCondition::DEFEAT:
    std::cout << Config::getInstance().getMessage("D_END_PLAYER_DEFEATED") << std::endl;
    break;

  case EndCondition::DRAW:
    std::cout << Config::getInstance().getMessage("D_END_DRAW_CARD") << std::endl;
    break;

  case EndCondition::MAX_ROUNDS:
    std::cout << Config::getInstance().getMessage("D_END_MAX_ROUNDS") << std::endl;

    //determine winner with who has more health left
    if (player1_.getCurrentHealth() > player2_.getCurrentHealth())
      winner_id_ = 1;
    else if (player1_.getCurrentHealth() < player2_.getCurrentHealth())
      winner_id_ = 2;

    break;

  default:
    break;
  }

  std::string result;

  //print result (if 0 -> tie (set in game constructor and not changed))
  if (winner_id_ == 0)
  {
    result = Config::getInstance().getMessage("D_TIE");
  }
  else
  {
    result = "Player " + std::to_string(winner_id_) + " has won! Congratulations!\n";
  }

  std::cout << result;
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;

  //open file, in append mode
  std::ofstream file(file_path, std::ios::app);
  if (!file.is_open())
    return false;

  file << '\n' << result;

  return true;
}


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

void Game::processRedraw(Command& command)
{
  if (!current_player_->getRedrawPossible())
    throw CommandError("E_REDRAW_DISABLED");

  if (current_player_->getDeckSize() < 2)
    throw CommandError("E_REDRAW_NOT_ENOUGH_CARDS");
  
  current_player_->redrawHand();
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
    board_active_ = true;

  printBoard();
}

void Game::processBattle(Command& command)
{
  //Syntax: battle <FIELD_SLOT> <BATTLE_SLOT>
  //move current player's creature from the field zone slot to the battle zone slot, a creature that was played this round cant battle, unless it has haste
  //check challenger, and haste

}

void Game::processCreature(Command& command) const
{
  // Syntax: creature <HAND_CARD_ID> <FIELD_SLOT>

  // Check if card exists:
  std::string card_id = command.getParameters().at(0);
  std::string field_slot = command.getParameters().at(1);

  if (Config::getInstance().getCard(card_id) == nullptr)
    throw CommandError("E_INVALID_CARD");

  // Check if field is playable
  if (!std::regex_match(field_slot, std::regex("O?(F|B)[1-7]")))
    throw CommandError("E_INVALID_SLOT");

  // Check if the card is in the players hand
  Card* card = current_player_->getHand().searchCard(card_id);
  
  if (card == nullptr)
    throw CommandError("E_NOT_IN_HAND");

  // Check if the card is a creature
  if (card->getType() != "Creature")
    throw CommandError("E_NOT_CREATURE");

  // Check if the field slot is in the players own field zone
  if (!std::regex_match(field_slot, std::regex("F[1-7]")))
    throw CommandError("E_NOT_IN_FIELD");

  // Check if there's already a creature on the field zone
  if (current_player_->isFieldSlotOccupied(field_slot) == true)
    throw CommandError("E_FIELD_OCCUPIED");

  // Check player has enough mana
  if (current_player_->getCurrentMana() - card->getManaCost() < 0)
    throw CommandError("E_NOT_ENOUGH_MANA");

  // Now play the creature
  current_player_->playCreature(card, field_slot);
}

void Game::processSpell(Command& command)
{
  // Syntax: spell <HAND_CARD_ID> <OPTIONAL_ADDITIONAL_PARAMETER>

  // Check if parameter count is zero
  if (command.getParameters().size() == 0)
    throw CommandError("E_MISSING_CARD");

  // Check if card exists:
  std::string card_id = command.getParameters().at(0);
  if (Config::getInstance().getCard(card_id) == nullptr)
    throw CommandError("E_INVALID_CARD");

  // Check if the card is in the players hand
  Card* card = current_player_->getHand().searchCard(card_id);
  if (card == nullptr)
    throw CommandError("E_NOT_IN_HAND");

  // Check if the card is a spell
  if (card->getType() != "Spell")
    throw CommandError("E_NOT_SPELL");

  // Individual Spell Type handlings
  Spell* spell_card = dynamic_cast<Spell*>(card);
  SpellType sub_type = spell_card->getSubtype();
  std::string second_param;

  if (sub_type == SpellType::GENERAL)
  {
    second_param = "";
    // Check if parameters are valid
    if (command.getParameters().size() != 1)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");
  }

  if (sub_type != SpellType::GENERAL)
    second_param = command.getParameters().at(1);

  if (sub_type == SpellType::TARGET)
  {
    // Check if parameters are valid
    if (command.getParameters().size() != 2)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");

    // Check if the target slot has valid syntax
    if (!std::regex_match(second_param, std::regex("O?(F|B)[1-7]")))
      throw CommandError("E_INVALID_SLOT_SPELL");

    // Check if the target slot has a creature
    if (second_param.at(0) == 'O')
    {
      // Remove the 'O'
      second_param.erase(second_param.begin());

      if (opposing_player_->isFieldSlotOccupied(second_param) == false)
        throw CommandError("E_TARGET_EMPTY");
    }
    else
    {
      if (current_player_->isFieldSlotOccupied(second_param) == false)
        throw CommandError("E_TARGET_EMPTY");
    }
  }

  if (sub_type == SpellType::GRAVEYARD)
  {
    // Check if parameters are valid
    if (command.getParameters().size() != 2)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");

    // Check if Graveyard contains the Card ID
    if (current_player_->getGraveyard().searchCard(second_param) == nullptr)
      throw CommandError("E_NOT_IN_GRAVEYARD");
  }

  // Check player has enough mana
  if (current_player_->getCurrentMana() - spell_card->getManaCost() < 0)
    throw CommandError("E_NOT_ENOUGH_MANA");

  // play spell
  current_player_->removeCardFromPile(PileType::HAND, spell_card->getId());

  // execute spell
  executeSpell(command);

  // Take away mana
  // we need to check x costs, and determine how much mana should get deduced
  current_player_->changeCurrentMana(0 - spell_card->getManaCost());

  // Print info
  std::string card_info_string = "I_" + card->getId();

  // Destroy spell after usage
  delete card;

  std::cout << Config::getInstance().getMessage(card_info_string) << std::endl;
}

void Game::executeSpell(Command& command)
{
  //Syntax: spell <HAND_CARD_ID> <OPTIONAL_ADDITIONAL_PARAMETER>
  std::string id = command.getParameters().at(0);
  std::string target;
  if (command.getParameters().size() == 2)
    target = command.getParameters().at(1);
  
  //General Spells
  if (id == "BTLCY")
    executeBattleCry();
  if (id == "METOR")
    executeMeteor();
  if (id == "FIRBL")
    executeFireball();
  
  //Target Spells
  if (id == "CLONE")
    executeClone(target);
  if (id == "CURSE")
    executeDeathCurse(target);
  if (id == "SHOCK")
    executeShock(target);
  if (id == "MOBLZ")
    executeMobilize(target);
  if (id == "RRUSH")
    executeRapidRush(target);
  if (id == "SHILD")
    executeShield(target);
  if (id == "AMPUT")
    executeAmputate(target);
  if (id == "FINAL")
    executeFinalAct(target);
  if (id == "LYLTY")
    executeLoyalty(target);
  if (id == "ZMBFY")
    executeZombify(target);
  if (id == "BLOOD")
    executeBloodlust(target);

  //Graveyard Spells
  if (id == "MEMRY")
    executeHeroicMemory(target);
  if (id == "REVIV")
    executeRevive(target);
}

void Game::executeBattleCry()
{
  //TODO
}

void Game::executeMeteor()
{
  //TODO
}

void Game::executeFireball()
{
  //TODO
}

void Game::executeClone(std::string target)
{
  //TODO
}

void Game::executeDeathCurse(std::string target)
{
  //TODO
}

void Game::executeShock(std::string target)
{
  //TODO
}

void Game::executeMobilize(std::string target)
{
  //TODO
}

void Game::executeRapidRush(std::string target)
{
  //TODO
}

void Game::executeShield(std::string target)
{
  //TODO
}

void Game::executeAmputate(std::string target)
{
  //TODO
}

void Game::executeFinalAct(std::string target)
{
  //TODO
}

void Game::executeLoyalty(std::string target)
{
  //TODO
}

void Game::executeZombify(std::string target)
{
  //TODO
}

void Game::executeBloodlust(std::string target)
{
  //TODO
}

void Game::executeHeroicMemory(std::string target)
{
  //TODO
}

void Game::executeRevive(std::string target)
{
  //TODO
}
