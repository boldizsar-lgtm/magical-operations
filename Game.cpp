#include "Game.hpp"
#include <fstream>

#define ZONE_WIDTH (7)

Game::Game(int max_rounds) : player1_{1}, player2_{2}, attacker_(&player1_), defender_(&player2_),
                             max_rounds_{max_rounds}, round_count_{0}, board_active_{true}
                             , end_condition_{EndCondition::DEFAULT}, winner_id_{0}
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

  //TODO: check refresh trait for creatures of current player, poisoned

  return true;
}


bool Game::startBattlePhase()
{
  std::cout << '\n' << Config::getInstance().getMessage("D_BORDER_BATTLE_PHASE") << '\n';

  for (int i = 1; i <= ZONE_WIDTH; i++)
  {
    std::cout << "---------------------------------------- SLOT " << i;
    std::cout << " -----------------------------------------\n";
    if (attacker_->getZone("BATTLE").isOccupied(i))
    {
      if (!startBattle(i))
        return false;
    }
  }

  std::cout << Config::getInstance().getMessage("D_BORDER_BATTLE_END") << '\n';

//TODO: besser strukturieren? -> processTraits(Player* player, Phase game_phase, char trait_type)
  removeTemporaryCreatures(attacker_);
  removeTemporaryCreatures(defender_);

  //all surviving creatures return to the first free field slot
  returnCreaturesToField();

  //undying creatures come back
  undeadCreaturesReenter(attacker_);
  undeadCreaturesReenter(defender_);

  player1_.turnAllEnteredOff();
  player2_.turnAllEnteredOff();
  return true;
}


void Game::undeadCreaturesReenter(Player* player)
{
  Creature* creature;
  Card* card;

  for (int i = 0; i < player->getGraveyard().getSize(); i++)
  {
    if ((card = player->getGraveyard().getPile().at(i)) != nullptr)
    {
      creature = dynamic_cast<Creature*>(card);
      if (creature != nullptr && creature->creatureHasTrait("Undying"))
      {
        player->moveToField(creature);
        std::cout << Config::getInstance().getMessage("I_UNDYING") << std::endl;
      }
    }
  }
}

void Game::removeTemporaryCreatures(Player* player)
{
  bool zones_have_temporary{false};

  //remove out of battle zone
  for (int i = 1; i <= ZONE_WIDTH; i++)
  {
    if (player->getZone("BATTLE").isOccupied(i))
    {
      if (player->getZone("BATTLE").getCreature(i)->creatureHasTrait("Temporary"))
      {
        player->moveToGraveyard('B', i);
        zones_have_temporary = true;
      }
    }
  }

  //remove out of field zone
  for (int i = 1; i <= ZONE_WIDTH; i++)
  {
    if (player->getZone("FIELD").isOccupied(i))
    {
      if (player->getZone("FIELD").getCreature(i)->creatureHasTrait("Temporary"))
      {
        player->moveToGraveyard('F', i);
        zones_have_temporary = true;
      }
    }
  }

  if (zones_have_temporary)
    std::cout << Config::getInstance().getMessage("I_TEMPORARY") << std::endl;
}

void Game::returnCreaturesToField()
{
  for (int i = 1; i <= ZONE_WIDTH; i++)
  {
    if (attacker_->getZone("BATTLE").isOccupied(i))
      attacker_->moveToField(i);
    if (defender_->getZone("BATTLE").isOccupied(i))
      defender_->moveToField(i);
  }
}


bool Game::directHitBattle(int column)
{
  int amount{0};

  amount = defender_->getCurrentHealth() - attacker_->getZone("BATTLE").getCreature(column)->getCurrentAttack();

  //reduce defenders health
  defender_->changeCurrentHealth((-attacker_->getZone("BATTLE").getCreature(column)->getCurrentAttack()));
  std::cout << Config::getInstance().getMessage("I_DIRECT") << std::endl;

  //end condition DEFEAT reached
  if (amount <= 0)
  {
    end_condition_ = EndCondition::DEFEAT;
    winner_id_ = attacker_->getId();
    return false;
  }

  return true;
}


bool Game::creatureFight(int column)
{
   //TODO: maybe split this function into to attacks with parameter int attack (1 or 2)??

  bool continue_battle{true};
  int amount{0};
  Player* opposing_creature_owner = defender_;
  Player* attacking_creature_owner = attacker_;
  Creature* creature_1 = attacker_->getZone("BATTLE").getCreature(column);
  Creature* creature_2 = defender_->getZone("BATTLE").getCreature(column);
  Creature* empty_creature = nullptr;
  Role first_attacker_role = Role::ATTACKER;
  bool empty_trait;
  bool first_strike{false};
  bool brutal{false};

  //Fight between two creatures begins
  std::cout << Config::getInstance().getMessage("I_FIGHT") << std::endl;

  //start Attack 1 (creature 1 attacks creature 2)
  std::cout << Config::getInstance().getMessage("D_ATTACK_1") << std::endl;

  //check which creatures have the first strike trait, change attack order accordingly (for the whole fight)
  first_attacker_role = processBattleTraits(creature_1, creature_2, first_strike, "First Strike");

  //switch who owns first opposing creature after first strike check
  if (first_attacker_role == Role::DEFENDER)
  {
    opposing_creature_owner = attacker_;
    attacking_creature_owner = defender_;
  }

  std::shared_ptr<Trait> temporary_trait;

  //check if first attacking creature has brutal trait
  processBattleTraits(creature_1, creature_2, brutal, "Brutal");

  //check if first attacking creature has venomous trait and add poisoned trait
  processBattleTraits(creature_1, creature_2, empty_trait, "Venomous");

  //check if second creature has lifesteal and processes it
  processBattleTraits(creature_1, empty_creature, empty_trait, "Lifesteal");

  //calculate remaining health/excess damage
  amount = creature_2->getCurrentHealth() - creature_1->getCurrentAttack();

  //decreasing creature health
  creature_2->changeCurrentHealth(-creature_1->getCurrentAttack());

  //if no first strike trait is active and creatures health is below 0, creature gains temporary trait,
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
        winner_id_ = attacking_creature_owner->getId();
        end_condition_ = EndCondition::DEFEAT;

        //TODO: (idk if second attack should still happen) if first strike defeats the opposing player return
        // if (first_strike)
        //   return false;
      }
      brutal = false;
    }

    temporary_trait = Config::getInstance().getTrait('T');
    creature_2->addTrait(temporary_trait);
  }


  //if second creature is dead because of first strike trait, battle does not continue
  if (continue_battle)
  {
    //start Attack 2 (creature 2 attacks creature 1)
    std::cout << Config::getInstance().getMessage("D_ATTACK_2") << std::endl;

    //switch opposing creature_owner
    if (first_attacker_role == Role::DEFENDER)
    {
      opposing_creature_owner = defender_;
      attacking_creature_owner = attacker_;
    }
    else
    {
      opposing_creature_owner = attacker_;
      attacking_creature_owner = defender_;
    }

    //check if second creature has brutal trait
    processBattleTraits(creature_2, creature_1, brutal, "Brutal");

    //check if second creature has venomous trait and add poisoned trait
    processBattleTraits(creature_2, creature_1, empty_trait, "Venomous");

    //check if second creature has lifesteal and processes it
    processBattleTraits(creature_2, empty_creature, empty_trait, "Lifesteal");

    //calculate remaining health/excess damage
    amount = creature_1->getCurrentHealth() - creature_2->getCurrentAttack();

    //decrease creature health
    creature_1->changeCurrentHealth(-creature_2->getCurrentAttack());

    if (amount <= 0)
    {
      temporary_trait = Config::getInstance().getTrait('T');
      creature_1->addTrait(temporary_trait);
      if (brutal)
      {
        opposing_creature_owner->changeCurrentHealth(amount);
        if (opposing_creature_owner->getCurrentHealth() <= 0)
        {
          winner_id_ = attacking_creature_owner->getId();
          end_condition_ = EndCondition::DEFEAT;
        }
      }
    }

  }

  if (end_condition_ == EndCondition::DEFEAT)
    return false;

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
      creature_2->addTrait(Config::getInstance().getTrait('P'));
      std::cout << Config::getInstance().getMessage("I_VENOMOUS") << std::endl;
    }
  }
  else if (trait_name == "Lifesteal")
  {
    if (creature_1->creatureHasTrait("Lifesteal"))
    {
      //increase current health of attacking creature
      creature_1->changeCurrentHealth(2);
      //if creatures health is now above zero it loses the temporary trait
      if (creature_1->getCurrentHealth() > 0 && creature_1->creatureHasTrait("Temporary"))
        creature_1->removeTrait("Temporary");
      std::cout << Config::getInstance().getMessage("I_LIFESTEAL") << std::endl;
    }
  }

  return first_attacker_role;
}

void Game::processTurnTraits()
{
  for (auto& creature : current_player_->getZone("FIELD").getSlots())
  {
    if (creature != nullptr)
    {
      int current_health = creature->getCurrentHealth();
      int base_health = creature->getBaseHealth();

      if (creature->creatureHasTrait("Poisoned"))
      {
        creatureTakesDamage(1, creature, current_player_);
        std::cout << Config::getInstance().getMessage("I_POISONED") << std::endl;
      }

      if (creature->creatureHasTrait("Regenerate") && (round_count_ % 2) == 1 && current_health < base_health)
      {
        creature->setCurrentHealth(base_health);
        std::cout << Config::getInstance().getMessage("I_REGENERATE") << std::endl;
      }
    }
  }

  // TODO: nothing todo, just wondering if this could even happen in battle stage here?
  for (auto& creature : current_player_->getZone("BATTLE").getSlots())
  {
    if (creature != nullptr)
    {
      int current_health = creature->getCurrentHealth();
      int base_health = creature->getBaseHealth();

      if (creature->creatureHasTrait("Poisoned"))
      {
        creatureTakesDamage(1, creature, current_player_);
        std::cout << Config::getInstance().getMessage("I_POISONED") << std::endl;
      }

      if (creature->creatureHasTrait("Regenerate") && round_count_ % 2 == 1 && current_health < base_health)
      {
        creature->setCurrentHealth(base_health);
        std::cout << Config::getInstance().getMessage("I_REGENERATE") << std::endl;
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

  //move dead creatures to graveyard (different from temporary creatures)
  if (attacker_->getZone("BATTLE").getCreature(column)->creatureHasTrait("Temporary"))
    attacker_->moveToGraveyard('B',column);
  if (defender_->getZone("BATTLE").getCreature(column)->creatureHasTrait("Temporary"))
    defender_->moveToGraveyard('B',column);

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

Creature* Game::getCreatureFromFieldString(std::string field_string)
{
  // Handle field parameter
  Player* target_player = current_player_;

  if (field_string.at(0) == 'O')
  {
    target_player = opposing_player_;
      
    // Remove the 'O'
    field_string.erase(field_string.begin());
  }

  char zone_type = field_string.at(0);
  
  int column = (field_string.at(1)) - '0';

  Creature* creature = target_player->getZone(zone_type).getCreature(column);

  return creature;
}

int Game::calculateXMana(Spell* spell_card, std::string second_param)
{
  Creature* target;

  if (spell_card->getId() == "MEMRY")
  {
    target = dynamic_cast<Creature*>(current_player_->getGraveyard().searchCard(second_param));

    float mana_cost = target->getManaCost() / 2;
    return (int)(mana_cost + 1);
  }

  target = getCreatureFromFieldString(second_param);

  if (spell_card->getId() == "CLONE")
  {
    float mana_cost = target->getManaCost() / 2;
    return (int)(mana_cost + 1);
  }

  else //(spell_card->getId() == "CURSE")
  {
    int mana_cost = target->getManaCost() + 1;
    return mana_cost;
  }

  return 0;
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
  std::cout << card->getName() << " [" << card->getId() << "] (" << card->getManaCostStringInfo() << " mana)";
  std::cout << std::endl << "Type: " << card->getType() << std::endl;

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

void Game::processRedraw()
{
  if (!current_player_->getRedrawPossible())
    throw CommandError("E_REDRAW_DISABLED");

  if (current_player_->getHand().getSize() < 2)
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
  int mana_cost = spell_card->getManaCost();

  if (sub_type == SpellType::GENERAL)
  {
    second_param = "";
    // Check if parameters are valid
    if (command.getParameters().size() != 1)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");
  }

  if (sub_type != SpellType::GENERAL)
    if(command.getParameters().size() > 1)
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
    // Check if parameters are valid
    if (command.getParameters().size() != 2)
      throw CommandError("E_INVALID_PARAM_COUNT_SPELL");

    // Check if Graveyard contains the Card ID
    if (current_player_->getGraveyard().searchCard(second_param) == nullptr)
      throw CommandError("E_NOT_IN_GRAVEYARD");
  }

  if (mana_cost == -1)
    mana_cost = calculateXMana(spell_card, second_param);

  // Check player has enough mana
  if (current_player_->getCurrentMana() - mana_cost < 0)
    throw CommandError("E_NOT_ENOUGH_MANA");

  // play spell
  current_player_->removeCardFromPile(PileType::HAND, spell_card->getId());

  // execute spell
  executeSpell(command);

  current_player_->changeCurrentMana(0 - mana_cost);

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

void Game::creatureTakesDamage(int amount, Player* target_player, char zone_type, int column)
{
  //target creature takes damage
  target_player->getZone(zone_type).getCreature(column)->changeCurrentHealth(- amount);

  //check if the creature would die and move it to the graveyard if it does
  if (target_player->getZone(zone_type).getCreature(column)->getCurrentHealth() < 1) //the if condition works poperly
    target_player->moveToGraveyard(zone_type, column); //this method doesnt work
}

void Game::creatureTakesDamage(int amount, Creature* creature, Player* owner)
{
  //target creature takes damage
  creature->changeCurrentHealth(- amount);

  //check if the creature would die and move it to the graveyard if it does
  if (creature->getCurrentHealth() < 1) //the if condition works poperly
    owner->moveToGraveyard(creature); //this method doesnt work
}


void Game::executeBattleCry()
{
  //allied creatures gain haste, temporary, and + 3 attack
  for (int i = 1; i <= ZONE_WIDTH; i++)
  {
    //check Field Zone
    if (current_player_->getZone('F').isOccupied(i))
    {
      //gains haste
      current_player_->getZone('F').getCreature(i)->addTrait(Config::getInstance().getTrait('H'));

      //gains temporary
      current_player_->getZone('F').getCreature(i)->addTrait(Config::getInstance().getTrait('T'));

      //attack increases by 3
      current_player_->getZone('F').getCreature(i)->changeCurrentAttack(3);
    }

    //check Battle Zone
    if (current_player_->getZone('B').isOccupied(i))
    {
      //gains haste
      current_player_->getZone('B').getCreature(i)->addTrait(Config::getInstance().getTrait('H'));

      //gains temporary
      current_player_->getZone('B').getCreature(i)->addTrait(Config::getInstance().getTrait('T'));

      //attack increases by 3
      current_player_->getZone('B').getCreature(i)->changeCurrentAttack(3);
    }
  }
}

void Game::executeMeteor()
{
  //each creature takes 3 damage
  for (int i = 1; i <= ZONE_WIDTH; i++)
  {
    if (opposing_player_->getZone('F').isOccupied(i))
      creatureTakesDamage(3, opposing_player_, 'F', i);

    if (opposing_player_->getZone('B').isOccupied(i))
      creatureTakesDamage(3, opposing_player_, 'B', i);

    if (current_player_->getZone('F').isOccupied(i))
      creatureTakesDamage(3, current_player_, 'F', i);

    if (current_player_->getZone('B').isOccupied(i))
      creatureTakesDamage(3, current_player_, 'B', i);
  }
}

void Game::executeFireball()
{
  //enemy creatures take 2 damage
  for (int i = 1; i <= ZONE_WIDTH; i++)
  {
    if (opposing_player_->getZone('F').isOccupied(i))
      creatureTakesDamage(2, opposing_player_, 'F', i);

    if (opposing_player_->getZone('B').isOccupied(i))
      creatureTakesDamage(2, opposing_player_, 'B', i);
  }
}

void Game::executeClone(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);
  
  Creature* clone = new Creature(*creature);

  current_player_->addToFirstEmptyField(clone);

  clone->addTrait(Config::getInstance().getTrait('H'));
  clone->addTrait(Config::getInstance().getTrait('T'));
}

void Game::executeDeathCurse(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);
  
  creature->addTrait(Config::getInstance().getTrait('T'));
}

void Game::executeShock(std::string target)
{
  //determine if it targets opponent's creatures, or own creatures
  Player* target_player = current_player_;
  if (target.size() == 3)
  {
    target_player = opposing_player_;
    target.erase(0, 1);
  }
  char zone_type = target.at(0);
  int column = (target.at(1)) - '0';

  //target creature takes 1 damage
  creatureTakesDamage(1, target_player, zone_type, column);
}

void Game::executeMobilize(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  //The target creature gains haste
  creature->addTrait(Config::getInstance().getTrait('H'));

  //its attack increases by 1
  creature->changeCurrentAttack(1);
}

void Game::executeRapidRush(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  //The target creature gains first strike
  creature->addTrait(Config::getInstance().getTrait('F'));

  //The target creature gains temporary
  creature->addTrait(Config::getInstance().getTrait('T'));

  //its attack increases by 2
  creature->changeCurrentAttack(2);
}

void Game::executeShield(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  //its health increases by 2
  creature->changeCurrentHealth(2);
}

void Game::executeAmputate(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  creature->removeFirstTrait();
}

void Game::executeFinalAct(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  //The target creature gains brutal
  creature->addTrait(Config::getInstance().getTrait('B'));

  //The target creature gains temporary
  creature->addTrait(Config::getInstance().getTrait('T'));

  //its attack increases by 3
  creature->changeCurrentAttack(3);
}

void Game::executeLoyalty(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  //The target creature gains haste
  creature->addTrait(Config::getInstance().getTrait('H'));

  //its health increases by 1
  creature->changeCurrentHealth(1);
}

void Game::executeZombify(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  //The target creature gains venomous
  creature->addTrait(Config::getInstance().getTrait('V'));

  //The target creature gains undying
  creature->addTrait(Config::getInstance().getTrait('U'));
}

void Game::executeBloodlust(std::string target)
{
  Creature* creature = getCreatureFromFieldString(target);

  //The target creature gains brutal
  creature->addTrait(Config::getInstance().getTrait('B'));

  //The target creature gains lifesteal
  creature->addTrait(Config::getInstance().getTrait('L'));

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
  creature->addTrait(Config::getInstance().getTrait('H'));

  //The target creature gains Temporary
  creature->addTrait(Config::getInstance().getTrait('T'));
}

void Game::executeRevive(std::string target)
{
  Creature* creature = dynamic_cast<Creature*>(current_player_->getGraveyard().searchCard(target));
  current_player_->moveToHand(creature);
}
