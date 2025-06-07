#include "Game.hpp"
#include <fstream>
#include <cmath>
#include <ranges>

const int TIE (0);
const int ZONE_WIDTH (7);
const int GENERAL_SPELL_PARAM_SIZE (1);
const int TARGET_SPELL_PARAM_SIZE (2);
const int GRAVEYARD_SPELL_PARAM_SIZE (2);
const int X_MANA_COST (-1);
const int MINIMUM_STARTING_HAND_SIZE (2);

Game::Game(int max_rounds) : player1_{1}, player2_{2}, attacker_(&player1_), defender_(&player2_),
                             max_rounds_{max_rounds}, round_count_{0}, board_active_{true}
                             , end_condition_{EndCondition::DEFAULT}, winner_id_{TIE}
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

  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    std::cout << "---------------------------------------- SLOT " << column;
    std::cout << " -----------------------------------------\n";
    if (attacker_->getZone("BATTLE").isOccupied(column))
    {
      if (!startBattle(column))
        return false;
    }
  }

  std::cout << Config::getInstance().getMessage("D_BORDER_BATTLE_END") << '\n';

  removeTemporaryCreatures(attacker_);
  removeTemporaryCreatures(defender_);

  //all surviving creatures return to the first free field slot
  returnCreaturesToField();

  //undying creatures come back
  undeadCreaturesReenter(attacker_);
  undeadCreaturesReenter(defender_);

  //the entered_this_round_ variable for all creatures is set to false
  attacker_->turnAllEnteredOff();
  defender_->turnAllEnteredOff();

  return true;
}

void Game::undeadCreaturesReenter(Player* player)
{
  Creature* creature;

  for (auto& card : player->getGraveyard().getPile())
  {
    if (player->isZoneFull('F'))
      return;

    creature = dynamic_cast<Creature*>(card);
    if (creature != nullptr && creature->creatureHasTrait("Undying"))
    {
      player->moveToField(creature);
      creature->removeTrait("Undying");
      std::cout << Config::getInstance().getMessage("I_UNDYING") << std::endl;
    }
  }
}

void Game::removeTemporaryCreatures(Player* player)
{
  //remove out of battle zone
  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    if (player->getZone("BATTLE").isOccupied(column))
    {
      if (player->getZone("BATTLE").getCreature(column)->creatureHasTrait("Temporary"))
      {
        player->moveToGraveyard('B', column);
        std::cout << Config::getInstance().getMessage("I_TEMPORARY") << std::endl;
      }
    }
  }

  //remove out of field zone
  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    if (player->getZone("FIELD").isOccupied(column))
    {
      if (player->getZone("FIELD").getCreature(column)->creatureHasTrait("Temporary"))
      {
        player->moveToGraveyard('F', column);
        std::cout << Config::getInstance().getMessage("I_TEMPORARY") << std::endl;
      }
    }
  }
}

void Game::returnCreaturesToField()
{
  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    if (attacker_->getZone("BATTLE").isOccupied(column))
      attacker_->moveToField(column);
    if (defender_->getZone("BATTLE").isOccupied(column))
      defender_->moveToField(column);
  }
}

bool Game::directHitBattle(int column)
{
  int amount{0};
  amount = defender_->getCurrentHealth() - attacker_->getZone("BATTLE").getCreature(column)->getCurrentAttack();

  //reduce defenders health
  defender_->changeCurrentHealth(-attacker_->getZone("BATTLE").getCreature(column)->getCurrentAttack());
  std::cout << Config::getInstance().getMessage("I_DIRECT") << std::endl;

  //end condition DEFEAT reached
  if (amount <= 0)
  {
    end_condition_ = EndCondition::DEFEAT;
    return false;
  }

  return true;
}

bool Game::startAttack(Creature*& creature_1, Creature*& creature_2, bool first_strike, bool& continue_battle,
  Player*& attacking_creature_owner)
{
  int amount{0};
  Player* opposing_creature_owner = defender_;
  Creature* empty_creature = nullptr;
  bool empty_trait{false};
  bool brutal{false};

  //switch who owns first opposing creature after first strike check
  if (attacking_creature_owner == opposing_creature_owner)
    opposing_creature_owner = attacker_;

  //check if first attacking creature has brutal trait
  processBattleTraits(creature_1, creature_2, brutal, "Brutal");

  //check if second creature has lifesteal and processes it
  processBattleTraits(creature_1, empty_creature, empty_trait, "Lifesteal");

  //check if first attacking creature has venomous trait and add poisoned trait
  processBattleTraits(creature_1, creature_2, empty_trait, "Venomous");

  //calculate remaining health/excess damage
  amount = creature_2->getCurrentHealth() - creature_1->getCurrentAttack();

  //decreasing creature health
  creature_2->changeCurrentHealth(-creature_1->getCurrentAttack());

  //if no first strike trait is active and creatures health is below 0, creatures dead_status_ set to true,
  //but battle continues
  if (amount <= 0)
  {
    if (first_strike)
    {
      //end battle early
      continue_battle = false;
    }

    //deal excess damage to opposing creatures owner
    if (brutal)
    {
      opposing_creature_owner->changeCurrentHealth(amount);
      if (opposing_creature_owner->getCurrentHealth() <= 0)
      {
        end_condition_ = EndCondition::DEFEAT;
      }
    }
    creature_2->setDeadStatus(true);
  }

  return true;
}

void Game::removeDeadCreatures(Creature*& creature_1, Creature*& creature_2, Role& first_attacker_role, int column)
{
  if (first_attacker_role == Role::ATTACKER)
  {
    if (creature_2->getDeadStatus())
      defender_->moveToGraveyard('B', column);
    if (creature_1->getDeadStatus())
      attacker_->moveToGraveyard('B', column);
  }
  else
  {
    if (creature_2->getDeadStatus())
      attacker_->moveToGraveyard('B', column);
    if (creature_1->getDeadStatus())
      defender_->moveToGraveyard('B', column);
  }
}

bool Game::creatureFight(int column)
{
  bool continue_battle{true};
  bool first_strike{false};
  Player* attacking_creature_owner = attacker_;
  Creature* creature_1 = attacker_->getZone("BATTLE").getCreature(column);
  Creature* creature_2 = defender_->getZone("BATTLE").getCreature(column);
  Role first_attacker_role = Role::ATTACKER;

  //Fight between two creatures begins
  std::cout << Config::getInstance().getMessage("I_FIGHT") << std::endl;

  //start Attack 1 (creature 1 attacks creature 2)
  std::cout << Config::getInstance().getMessage("D_ATTACK_1") << std::endl;

  //check which creatures have the first strike trait, change attack order accordingly (for the whole fight)
  first_attacker_role = processBattleTraits(creature_1, creature_2, first_strike, "First Strike");

  if (first_attacker_role == Role::DEFENDER)
    attacking_creature_owner = defender_;

  startAttack(creature_1, creature_2, first_strike, continue_battle, attacking_creature_owner);

  //if second creature is dead because of first strike trait, battle does not continue
  if (continue_battle)
  {
    //start Attack 2 (creature 2 attacks creature 1)
    std::cout << Config::getInstance().getMessage("D_ATTACK_2") << std::endl;

    //switch opposing creature owner
    if (first_attacker_role == Role::DEFENDER)
    {
      attacking_creature_owner = attacker_;
    }
    else
    {
      attacking_creature_owner = defender_;
    }

    startAttack(creature_2, creature_1, first_strike, continue_battle, attacking_creature_owner);
  }

  if (end_condition_ == EndCondition::DEFEAT)
    return false;

  removeDeadCreatures(creature_1, creature_2, first_attacker_role, column);

  return true;
}


Role Game::processBattleTraits(Creature*& creature_1, Creature*& creature_2, bool& trait, std::string trait_name)
{
  Role first_attacker_role = Role::ATTACKER;

  if (trait_name == "First Strike")
  {
    Creature* temp;
    //Check if only one of the fighting creatures has First Strike trait and change attack order accordingly
    if (creature_1->creatureHasTrait("First Strike") && !creature_2->creatureHasTrait("First Strike"))
    {
      trait = true;
    }
    else if (creature_2->creatureHasTrait("First Strike") && !creature_1->creatureHasTrait("First Strike"))
    {
      //switch first attacker role
      first_attacker_role = Role::DEFENDER;
      trait = true;
      //switch attack order
      temp = creature_1;
      creature_1 = creature_2;
      creature_2 = temp;
    }
    if (trait)
    {
      std::cout << Config::getInstance().getMessage("I_FIRST_STRIKE") << std::endl;
    }
  }
  else if (trait_name == "Brutal")
  {
    if (creature_1->creatureHasTrait("Brutal"))
    {
      if ((creature_2->getCurrentHealth() - creature_1->getCurrentAttack()) <= 0)
        std::cout << Config::getInstance().getMessage("I_BRUTAL") << std::endl;

      trait = true;
    }
  }
  else if (trait_name == "Venomous")
  {
    if (creature_1->creatureHasTrait("Venomous"))
    {
      //add trait poisoned to opposing creature
      *creature_2 += (Config::getInstance().getTrait('P'));
      std::cout << Config::getInstance().getMessage("I_VENOMOUS") << std::endl;
    }
  }
  else if (trait_name == "Lifesteal")
  {
    if (creature_1->creatureHasTrait("Lifesteal"))
    {
      //increase current health of attacking creature
      creature_1->changeCurrentHealth(LIFESTEAL_HEALTH_GAIN);

      //if creatures health is now above zero, the creature dead trait gets set to false
      if (creature_1->getCurrentHealth() > 0)
        creature_1->setDeadStatus(false);

      std::cout << Config::getInstance().getMessage("I_LIFESTEAL") << std::endl;
    }
  }

  return first_attacker_role;
}

void Game::processTurnTraits()
{
  for (auto& creature : current_player_->getZone("BATTLE").getSlots())
  {
    if (creature != nullptr)
    {
      int current_health = creature->getCurrentHealth();
      int base_health = creature->getBaseHealth();

      if (creature->creatureHasTrait("Regenerate") && round_count_ % 2 == 1 && current_health < base_health)
      {
        creature->setCurrentHealth(base_health);
        std::cout << Config::getInstance().getMessage("I_REGENERATE") << std::endl;
      }

      if (creature->creatureHasTrait("Poisoned"))
      {
        creatureTakesDamage(POISONED_DAMAGE, creature, current_player_);
        std::cout << Config::getInstance().getMessage("I_POISONED") << std::endl;
      }
    }
  }

  for (auto& creature : current_player_->getZone("FIELD").getSlots())
  {
    if (creature != nullptr)
    {
      int current_health = creature->getCurrentHealth();
      int base_health = creature->getBaseHealth();

      if (creature->creatureHasTrait("Regenerate") && (round_count_ % 2) == 1 && current_health < base_health)
      {
        creature->setCurrentHealth(base_health);
        std::cout << Config::getInstance().getMessage("I_REGENERATE") << std::endl;
      }

      if (creature->creatureHasTrait("Poisoned"))
      {
        creatureTakesDamage(POISONED_DAMAGE, creature, current_player_);
        std::cout << Config::getInstance().getMessage("I_POISONED") << std::endl;
      }
    }
  }
}

bool Game::startBattle(int column)
{
  //Direct hit battle
  if (!defender_->getZone("BATTLE").isOccupied(column))
  {
    //returns false if end condition DEFEAT is reached
    if(!directHitBattle(column))
      return false;
    else
      return true;
  }

  //Fight between two creatures, returns false if end condition DEFEAT is reached
  if (!creatureFight(column))
    return false;

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
    processRedraw();

  if (command_type == CommandType::STATUS)
    printStatus();

  if (command_type == CommandType::DONE)
  {
    //player ends their turn, the board is printed,
    current_player_->setDoneStatus(true);
    processTurnTraits();
    printBoard();
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

  //NEW a3 commands added:
  //print team name
  if (command_type == CommandType::WHOAMI)
  {
    std::cout << "Team: Giveus Ana" << std::endl;
  }

  //print board, status, and hand
  if (command_type == CommandType::SYNC)
  {
    printBoard();
    printStatus();
    std::cout << "Player 1" << std::endl;
    player1_.printHand();
    std::cout << "Player 2" << std::endl;
    player2_.printHand();
  }

  //AI automatically selects single command to execute next
  if (command_type == CommandType::PLAY)
  {
    letAiPlay();
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
      break;

    default:
      break;
  }

  if (end_condition_ == EndCondition::DEFEAT || end_condition_ == EndCondition::MAX_ROUNDS)
  {
    //determine winner with who has more health left
    if (player1_.getCurrentHealth() > player2_.getCurrentHealth())
      winner_id_ = 1;
    else if (player1_.getCurrentHealth() < player2_.getCurrentHealth())
      winner_id_ = 2;
  }

  std::string result;

  //print result
  if (winner_id_ == TIE)
  {
    result = Config::getInstance().getMessage("D_TIE") + '\n';
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

Creature* Game::getCreatureFromInputString(std::string input_string)
{
  // Handle field parameter
  Player* target_player = current_player_;

  if (input_string.at(0) == 'O')
  {
    target_player = opposing_player_;

    // Remove the 'O'
    input_string.erase(input_string.begin());
  }

  char zone_type = input_string.at(0);
  
  int column = (input_string.at(1)) - '0';

  Creature* creature = target_player->getZone(zone_type).getCreature(column);

  return creature;
}

int Game::calculateXMana(Spell* spell_card, std::string second_param)
{
  Creature* target;

  if (spell_card->getId() == "MEMRY")
  {
    target = dynamic_cast<Creature*>(current_player_->getGraveyard().searchCard(second_param));

    float mana_cost = static_cast<float>(target->getManaCost()) / 2.0;

    return static_cast<int>(ceil(mana_cost));
  }

  target = getCreatureFromInputString(second_param);

  if (spell_card->getId() == "CLONE")
  {
    float mana_cost =  static_cast<float>(target->getManaCost()) / 2.0;

    return static_cast<int>(ceil(mana_cost));
  }

  else
  {
    int mana_cost = target->getManaCost() + 1;
    return mana_cost;
  }

  return 0;
}

void Game::creatureTakesDamage(int amount, Player* target_player, char zone_type, int column)
{
  //target creature takes damage
  target_player->getZone(zone_type).getCreature(column)->changeCurrentHealth(- amount);

  //check if the creature would die and move it to the graveyard if it does
  if (target_player->getZone(zone_type).getCreature(column)->getCurrentHealth() < 1)
    target_player->moveToGraveyard(zone_type, column);
}

void Game::creatureTakesDamage(int amount, Creature* creature, Player* owner)
{
  //target creature takes damage
  creature->changeCurrentHealth(- amount);

  //check if the creature would die and move it to the graveyard if it does
  if (creature->getCurrentHealth() < 1)
    owner->moveToGraveyard(creature);
}


///COMMANDS

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
  std::cout << card->getName() << " [" << card->getId() << "] (" << card->getManaCostStringInfo() << " mana)";
  std::cout << std::endl << "Type: " << card->getType() << std::endl;

  // Print card specifics
  if (card->getType() == "Creature")
  {
    // Cast a creature and print everything
    // Traits are handled in Creature Class
    Creature* creature = dynamic_cast<Creature*>(card);
    std::cout << "Base Attack: " << creature->getBaseAttack() << std::endl;
    std::cout << "Base Health: " << creature->getBaseHealth() << std::endl;
    std::cout << "Base Traits: ";
    creature->printBaseTraitList();
  }
  else
  {
    // Cast a spell (ba-dum-tss) and print everything
    Spell* spell = dynamic_cast<Spell*>(card);
    std::cout << "Effect: " << spell->getEffect() << std::endl;
  }

  // Print end border
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;
}

void Game::processRedraw()
{
  if (!current_player_->getRedrawPossible())
    throw CommandError("E_REDRAW_DISABLED");

  if (current_player_->getHand().getSize() < MINIMUM_STARTING_HAND_SIZE)
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

  std::string field_slot = command.getParameters().at(0);
  std::string battle_slot = command.getParameters().at(1);

  // Check if field or battle ares playable
  if (!std::regex_match(field_slot, std::regex("O?(F|B)[1-7]")) || !std::regex_match(battle_slot, std::regex("O?(F|B)[1-7]")) )
    throw CommandError("E_INVALID_SLOT");

  // Check if field zone is valid
  if (field_slot.at(0) == 'O' || field_slot.at(0) == 'B')
    throw CommandError("E_NOT_IN_FIELD");

  // Check if field slot has a creature
  if (current_player_->isFieldSlotOccupied(field_slot) == false)
    throw CommandError("E_FIELD_EMPTY");

  // Check if creature entered board the same round and does not have haste
  Creature* creature = current_player_->getZone("FIELD").getCreature(field_slot.at(1) - '0');
  bool has_haste = creature->creatureHasTrait("Haste");
  bool has_challenger = creature->creatureHasTrait("Challenger");

  if (creature->getEnter() == true && has_haste == false)
    throw CommandError("E_CREATURE_CANNOT_BATTLE");

  // Check if battle Zone is valid
  if (battle_slot.at(0) == 'O' || battle_slot.at(0) == 'F')
    throw CommandError("E_NOT_IN_BATTLE");

  // Check if battle zone is occupied
  if (current_player_->isFieldSlotOccupied(battle_slot) == true)
    throw CommandError("E_BATTLE_OCCUPIED");

  current_player_->moveToBattle(field_slot, battle_slot);

  // Print Haste
  if (has_haste == true && creature->getEnter() == true)
    std::cout << Config::getInstance().getMessage("I_HASTE") << std::endl;

  // Execute Challenger Trait
  if (has_challenger == true)
  {
    std::string opponent_battle = battle_slot;
    battle_slot.at(0) = 'F';
    std::string opponent_field = battle_slot;

    if (opposing_player_->isFieldSlotOccupied(opponent_field) == true &&
        opposing_player_->isFieldSlotOccupied(opponent_battle) == false)
    {
      opposing_player_->moveToBattle(opponent_field, opponent_battle);
      std::cout << Config::getInstance().getMessage("I_CHALLENGER") << std::endl;
    }
  }
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

  if (command.getParameters().size() == 0)
    throw CommandError("E_MISSING_CARD");

  std::string card_id = command.getParameters().at(0);
  if (Config::getInstance().getCard(card_id) == nullptr)
    throw CommandError("E_INVALID_CARD");

  Card* card = current_player_->getHand().searchCard(card_id);
  if (card == nullptr)
    throw CommandError("E_NOT_IN_HAND");

  if (card->getType() != "Spell")
    throw CommandError("E_NOT_SPELL");

  Spell* spell_card = dynamic_cast<Spell*>(card);
  SpellType sub_type = spell_card->getSubtype();
  std::string second_param;
  int mana_cost = spell_card->getManaCost();

  if (sub_type == SpellType::GENERAL)
  {
    second_param = "";
    if (command.getParameters().size() != GENERAL_SPELL_PARAM_SIZE)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");
  }

  if (sub_type != SpellType::GENERAL)
    if(command.getParameters().size() > 1)
      second_param = command.getParameters().at(1);

  if (sub_type == SpellType::TARGET)
  {
    if (command.getParameters().size() != TARGET_SPELL_PARAM_SIZE)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");

    if (!std::regex_match(second_param, std::regex("O?(F|B)[1-7]")))
      throw CommandError("E_INVALID_SLOT_SPELL");

    if (second_param.at(0) == 'O')
    {
      if (opposing_player_->isFieldSlotOccupied(second_param.substr(1)) == false)
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
    if (command.getParameters().size() != GRAVEYARD_SPELL_PARAM_SIZE)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");

    if (current_player_->getGraveyard().searchCard(second_param) == nullptr)
      throw CommandError("E_NOT_IN_GRAVEYARD");
  }

  if (mana_cost == X_MANA_COST)
    mana_cost = calculateXMana(spell_card, second_param);

  if (current_player_->getCurrentMana() - mana_cost < 0)
    throw CommandError("E_NOT_ENOUGH_MANA");

  current_player_->removeCardFromPile(PileType::HAND, spell_card->getId());

  executeSpell(command, card, mana_cost);
}

void Game::executeSpell(Command& command, Card* card, int mana_cost)
{
  //Syntax: spell <HAND_CARD_ID> <OPTIONAL_ADDITIONAL_PARAMETER>

  // Reduce player's mana
  current_player_->changeCurrentMana(0 - mana_cost);

  // Print info
  std::string card_info_string = "I_" + card->getId();
  std::cout << Config::getInstance().getMessage(card_info_string) << std::endl;

  // Destroy spell after usage
  delete card;

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


///SPELLS

void Game::executeBattleCry()
{
  //allied creatures gain haste, temporary, and + 3 attack
  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    //check Field Zone
    if (current_player_->getZone('F').isOccupied(column))
    {
      //gains haste
      *current_player_->getZone('F').getCreature(column) += (Config::getInstance().getTrait('H'));

      //gains temporary
      *current_player_->getZone('F').getCreature(column) += (Config::getInstance().getTrait('T'));

      //attack increases by 3
      current_player_->getZone('F').getCreature(column)->changeCurrentAttack(BATTLE_CRY_ATTACK_GAIN);
    }

    //check Battle Zone
    if (current_player_->getZone('B').isOccupied(column))
    {
      //gains haste
      *current_player_->getZone('B').getCreature(column) += (Config::getInstance().getTrait('H'));

      //gains temporary
      *current_player_->getZone('B').getCreature(column) += (Config::getInstance().getTrait('T'));

      //attack increases by 3
      current_player_->getZone('B').getCreature(column)->changeCurrentAttack(BATTLE_CRY_ATTACK_GAIN);
    }
  }
}

void Game::executeMeteor()
{
  //each creature takes 3 damage
  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    if (opposing_player_->getZone('F').isOccupied(column))
      creatureTakesDamage(METEOR_DAMAGE, opposing_player_, 'F', column);

    if (opposing_player_->getZone('B').isOccupied(column))
      creatureTakesDamage(METEOR_DAMAGE, opposing_player_, 'B', column);

    if (current_player_->getZone('F').isOccupied(column))
      creatureTakesDamage(METEOR_DAMAGE, current_player_, 'F', column);

    if (current_player_->getZone('B').isOccupied(column))
      creatureTakesDamage(METEOR_DAMAGE, current_player_, 'B', column);
  }
}

void Game::executeFireball()
{
  //enemy creatures take 2 damage
  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    if (opposing_player_->getZone('F').isOccupied(column))
      creatureTakesDamage(FIREBALL_DAMAGE, opposing_player_, 'F', column);

    if (opposing_player_->getZone('B').isOccupied(column))
      creatureTakesDamage(FIREBALL_DAMAGE, opposing_player_, 'B', column);
  }
}

void Game::executeClone(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  Creature* clone = new Creature(*creature);

  current_player_->addToFirstEmptyField(clone);

  *clone += (Config::getInstance().getTrait('H'));
  *clone += (Config::getInstance().getTrait('T'));
}

void Game::executeDeathCurse(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  *creature += (Config::getInstance().getTrait('T'));
}

void Game::executeShock(std::string target)
{
  Player* owner = current_player_;
  if (target.at(0) == 'O')
    owner = opposing_player_;
  
  Creature* creature = getCreatureFromInputString(target);

  //target creature takes 1 damage
   creatureTakesDamage(SHOCK_DAMAGE, creature, owner);
}

void Game::executeMobilize(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  //The target creature gains haste
  *creature += (Config::getInstance().getTrait('H'));

  //its attack increases by 1
  creature->changeCurrentAttack(MOBILIZE_ATTACK_GAIN);
}

void Game::executeRapidRush(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  //The target creature gains first strike
  *creature += (Config::getInstance().getTrait('F'));

  //The target creature gains temporary
  *creature += (Config::getInstance().getTrait('T'));

  //its attack increases by 2
  creature->changeCurrentAttack(RAPID_RUSH_ATTACK_GAIN);
}

void Game::executeShield(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  //its health increases by 2
  creature->changeCurrentHealth(SHIELD_HEALTH_GAIN);
}

void Game::executeAmputate(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  creature->removeFirstTrait();
}

void Game::executeFinalAct(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  //The target creature gains brutal
  *creature += (Config::getInstance().getTrait('B'));

  //The target creature gains haste
  *creature += (Config::getInstance().getTrait('H'));

  //The target creature gains temporary
  *creature += (Config::getInstance().getTrait('T'));

  //its attack increases by 3
  creature->changeCurrentAttack(FINAL_ACT_ATTACK_GAIN);
}

void Game::executeLoyalty(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  //The target creature gains haste
  *creature += (Config::getInstance().getTrait('H'));

  //its health increases by 1
  creature->changeCurrentHealth(LOYALTY_HEALTH_GAIN);
}

void Game::executeZombify(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  //The target creature gains venomous
  *creature += (Config::getInstance().getTrait('V'));

  //The target creature gains undying
  *creature += (Config::getInstance().getTrait('U'));
}

void Game::executeBloodlust(std::string target)
{
  Creature* creature = getCreatureFromInputString(target);

  //The target creature gains brutal
  *creature += (Config::getInstance().getTrait('B'));

  //The target creature gains lifesteal
  *creature += (Config::getInstance().getTrait('L'));

  //its health is set to half its current health (rounded up)
  int new_health = creature->getCurrentHealth();
  if (new_health % 2 != 0)
    new_health++;
  new_health /= 2;
  creature->setCurrentHealth(new_health);
}

void Game::executeHeroicMemory(std::string target)
{
  Creature* creature = dynamic_cast<Creature*>(current_player_->getGraveyard().searchCard(target));
  current_player_->moveToField(creature);

  //The target creature gains Haste
  *creature += (Config::getInstance().getTrait('H'));

  //The target creature gains Temporary
  *creature += (Config::getInstance().getTrait('T'));
}

void Game::executeRevive(std::string target)
{
  Creature* creature = dynamic_cast<Creature*>(current_player_->getGraveyard().searchCard(target));
  current_player_->moveToHand(creature);
}


//-----------------------------------------------------------------------------------------------------
//NEW a3 method:
void Game::letAiPlay()
{
  std::vector<std::string> command;

  int creature_priority = 0;
  std::vector<std::string> creature_command = {"creature", ""};
  //syntax: creature <id> <fieldzone>

  int spell_priority = 0;
  std::vector<std::string> spell_command = {"spell", "", ""};
  //syntax: spell <id> <optional>

  int battle_priority = 0;
  std::vector<std::string> battle_command = {"battle", "", ""};
  //syntax: battle <fieldzone> <battlezone>


  //find first empty field zone
  creature_command.push_back("f" + std::to_string(current_player_->getFirstEmptyFieldColumn()));

  //if the board is not full get creature priority
  if (creature_command.at(2) != "f0")
    getCreaturePriority(creature_priority, creature_command);

  getSpellPriority(spell_priority, spell_command);

  getBattlePriority(battle_priority, battle_command);


  //creature
  if (creature_priority > spell_priority && creature_priority > battle_priority)
    command = creature_command;
  //spell
  else if (spell_priority > creature_priority && spell_priority > battle_priority)
    command = spell_command;
  //battle
  else if (battle_priority > spell_priority && battle_priority > creature_priority)
    command = battle_command;
  //redraw
  else if (current_player_->getRedrawPossible() && current_player_->getHand().getSize() > 5)
    command = {"redraw"};
  //done
  else
    command = {"done"};


  printAiCommand(command);

  Command ai(command);
  processCommand(ai);
}

//------------------
void Game::getCreaturePriority(int& priority, std::vector<std::string>& command)
{
  for (Card* card : current_player_->getHand().getPile())
  {
    int current_priority = 0;

    //if card is too expensive, skip it
    if (card->getManaCost() > current_player_->getCurrentMana())
      continue;

    if (card->getType() == "Creature")
    {
      Creature* creature = dynamic_cast<Creature*>(card);
      //Increase CREATURE PRIORITY
      current_priority += creature->getBaseAttack() + creature->getBaseHealth();
    }

    //if current creature's priority is higher than the saved creature's save it instead
    if (current_priority > priority)
    {
      priority = current_priority;
      command.at(1) = card->getId();
    }
  }
}

//------------------
void Game::getSpellPriority(int& priority, std::vector<std::string>& command)
{
  for (Card* card : current_player_->getHand().getPile())
  {
    int current_priority = 0;

    //if card is too expensive, skip it
    if (card->getManaCost() > current_player_->getCurrentMana())
      continue;

    if (card->getType() == "Spell")
    {
      Spell* spell = dynamic_cast<Spell*>(card);
      //Increase SPELL PRIORITY
    }

    //if current spell's priority is higher than the saved spell's save it instead
    if (current_priority > priority)
    {
      priority = current_priority;
      command.at(1) = card->getId();
    }
  }
}

//------------------
void Game::getBattlePriority(int& priority, std::vector<std::string>& command)
{
  // TODO: maybe look for creatures that can take the damage of brutal creatures
  // TODO: place weakest creatures infront of creatures

  // save the health of all creatures of the field zone of the current player
  auto health_values = current_player_->getZone('F').getSlots()
  | std::views::filter([this] (Creature* creature) -> bool
  {
    if (creature && (!creature->getEnter() || creature->creatureHasTrait("Haste"))) 
    {
      return true;
    }
    return false;
  })
  | std::views::transform([] (Creature* creature) -> int {return creature->getCurrentHealth();});

  // get the max health value
  int max_health = 0;
  if (std::ranges::distance(health_values) != 0)
    max_health = *std::ranges::max_element(health_values);


  std::vector<Creature*> opposing_battle_slots = opposing_player_->getZone('B').getSlots();
  std::vector<int> health_opposer;

  for (auto& creature : opposing_battle_slots)
  {
    if (creature)
    health_opposer.push_back(creature->getCurrentHealth());
  }

  //TODO where to check for this (+ brutal value)
  int min_health_opposer = 0;
  if (health_opposer.size() != 0)
    min_health_opposer = *std::ranges::min_element(health_opposer);

  for (int column = 1; column <= ZONE_WIDTH; column++)
  {
    int current_priority = 0;

    //if fieldzone is not occupied, skip this column
    if(!current_player_->getZone('F').isOccupied(column))
      continue;

    //if the creature entered this turn and doesnt have haste, skkip this   column
    if (current_player_->getZone('F').getCreature(column)->getEnter() &&
        !current_player_->getZone('F').getCreature(column)->creatureHasTrait("Haste"))
      continue;

    int battle_slot_to_defend = 1;

    // Logic for Defense in Battle Zone
    if(current_player_->getRoleString() == "Defender")
    {
      // filter that keeps current players field creatures that have brutal trait (maybe switch this to a count func?)
      auto defender_brutal_creatures =  current_player_->getZone('F').getSlots()
      | std::views::filter([this] (Creature* creature) -> bool
        {
          if (creature != nullptr && creature->creatureHasTrait("Brutal"))
            return true; // keep creatures that have brutal trait
          return false;
        });

      // filter that keeps creatures that are undefended
      auto undefended_creatures = opposing_player_->getZone('B').getSlots()
      | std::views::filter([this] (Creature* creature) -> bool 
      {
        int opposing_slot = opposing_player_->getCreatureColumn('B', creature);

        if (creature != nullptr && !current_player_->getZone('B').isOccupied(opposing_slot))
        {
          return true; // keep creatures that are undefended
        }
        return false;
      });

      // check if there are brutal creatures that could deal damage to current player (the opposing battle slot is still empty)
      auto undefended_brutal_creatures =  opposing_battle_slots
      | std::views::filter([this] (Creature* creature) -> bool
        {
          int opposing_slot = opposing_player_->getCreatureColumn('B', creature);

          if (creature != nullptr && creature->creatureHasTrait("Brutal") && !current_player_->getZone('B').isOccupied(opposing_slot))
            return true; // keep creatures that have brutal trait and are undefended
          return false;
        });

      Creature* attacking_creature = current_player_->getZone('F').getCreature(column);

      // determine which slot to defend (brutal creatures defended by creature with highest health)
      if (std::ranges::distance(undefended_brutal_creatures) != 0)
      {
        // defense against brutal has highest priority
        current_priority += 2;
        if (max_health == attacking_creature->getCurrentHealth())  
        {
          battle_slot_to_defend = determineBestDefenseSlot(attacking_creature, undefended_brutal_creatures, min_health_opposer);
        }
        else
          continue;
      }
      else if(std::ranges::distance(undefended_creatures) != 0)
      {
        battle_slot_to_defend = determineBestDefenseSlot(attacking_creature, undefended_creatures, min_health_opposer);
        // if the defender has a brutal creature that creature gains priority
        if (attacking_creature->creatureHasTrait("Brutal"))
          current_priority++;
      }
      else
        return;
    }

    //increase BATTLE PRRIORITY
    current_priority++;

    //if current creature's priority is higher than the saved creature's save it instead
    if (current_priority > priority)
    {
      priority = current_priority;
      command.at(1) = "f" + std::to_string(column);

      if (current_player_->getRoleString() == "Attacker")
        command.at(2) = "b" + std::to_string(column);
      else //current_player is defender
        command.at(2) = "b" + std::to_string(battle_slot_to_defend); //CHANGE, it should block instead
    }
  }
}

int Game::determineBestDefenseSlot(Creature* attacking_creature, auto& undefended_creatures, int min_health_opposer)
{
  int max_attack_value = 0;
  int battle_slot_to_defend = 0;

  for (auto creature : undefended_creatures)
  {

    // switch column if there is a creture with a higher attack value
    if (attacking_creature->creatureHasTrait("Brutal") && creature->getCurrentHealth() == min_health_opposer)
    {
      battle_slot_to_defend = opposing_player_->getCreatureColumn('B', creature);
      return battle_slot_to_defend;
    }
    if (creature->getCurrentAttack() > max_attack_value)  
    {
      max_attack_value = creature->getCurrentAttack();
      battle_slot_to_defend = opposing_player_->getCreatureColumn('B', creature);
    }
  }

  return battle_slot_to_defend;
}

//------------------
void Game::printAiCommand(std::vector<std::string>& command)
{
  std::cout << "AI:";

  for(std::string& parameter : command)
  {
    Utils::toLower(parameter);

    std::cout << ' ' << parameter;

    Utils::toUpper(parameter);
  }

  std::cout << std::endl;
}

