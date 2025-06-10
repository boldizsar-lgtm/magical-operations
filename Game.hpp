//---------------------------------------------------------------------------------------------------------------------
// Game.hpp/Game.cpp
//
// The Game class contains all relevant information on the current game and methods that process the different aspects
// of the each game phase (setup, battle, final) and finally writes the result of the game to the input file.
//
// Group: 026
//
// Author: Boldizsar Kiss, 12417350,
//         Lenon Osmani, 12303674,
//         Katharina Hartmann, 12219843
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <regex>
#include "Config.hpp"
#include "Player.hpp"
#include "Command.hpp"
#include "CommandError.hpp"
#include "CommandLine.hpp"


enum Gain
{
  LIFESTEAL_HEALTH_GAIN = 2,
  BATTLE_CRY_ATTACK_GAIN = 3,
  MOBILIZE_ATTACK_GAIN = 1,
  RAPID_RUSH_ATTACK_GAIN = 2,
  SHIELD_HEALTH_GAIN = 2,
  FINAL_ACT_ATTACK_GAIN = 3,
  LOYALTY_HEALTH_GAIN = 1
};

enum Damage
{
  POISONED_DAMAGE = 1,
  METEOR_DAMAGE = 3,
  FIREBALL_DAMAGE = 2,
  SHOCK_DAMAGE = 1
};

enum class EndCondition
{
  DEFAULT,
  DEFEAT,
  DRAW,
  MAX_ROUNDS,
  QUIT
};

class Game
{
protected:
  Player player1_;
  Player player2_;
  Player* current_player_;
  Player* opposing_player_;
  Player* attacker_;
  Player* defender_;
  int max_rounds_;
  int round_count_;
  bool board_active_;
  EndCondition end_condition_;
  int winner_id_;

public:
  // Constructor, Copy-Assignment-Operator, Destructor
  Game(int max_rounds);
  Game(Game&) = default;
  ~Game() = default;

  
  //------------------------------------------------------------------------------------------------------------------
  // starts and handles a single round of the game, updates all needed values
  //
  // @return true if the game continues
  //         false if an end condition is met
  //
  bool startRound();

  //------------------------------------------------------------------------------------------------------------------
  // starts the setup phase of the game
  //
  // @return true if the game continues
  //         false if an end condition is met in doCommandLoop()
  //
  bool startSetupPhase();

  //------------------------------------------------------------------------------------------------------------------
  // handles the command input loop of the game
  //
  // @param cli the command line interface object
  //
  // @return true if the game continues
  //         false if quit is entered
  //
  bool doCommandLoop(CommandLine& cli);

  //------------------------------------------------------------------------------------------------------------------
  // starts the battle phase of the game
  //
  // @return true if the game continues
  //         false if an end condition is met in startBattle()
  //
  bool startBattlePhase();

  //------------------------------------------------------------------------------------------------------------------
  // handles Creatures with the "Undying" trait (not) getting added back at the end of a round
  //
  // @param Player the player whose creatures are to reenter
  //
  void undeadCreaturesReenter(Player* player);
  
  //------------------------------------------------------------------------------------------------------------------
  // handles Creatures with the "Temporary" trait getting removed at the end of a round
  //
  // @param Player the player whose creatures are to be checked
  //
  void removeTemporaryCreatures(Player* player);

  //------------------------------------------------------------------------------------------------------------------
  // handles the returning of the creatures to their field zones from the battle zones at the end of a round
  //
  void returnCreaturesToField();

  //------------------------------------------------------------------------------------------------------------------
  // handles direct hits on a player and checks if the player is defeated
  //
  // @param column the column of the creature that's attacking 
  // 
  // @return true if the game continues
  //         false if the player is defeated
  //
  bool directHitBattle(int column);

  //------------------------------------------------------------------------------------------------------------------
  // starts a singular attack between two creatures in the same battle slot
  //
  // @param creature_1 creature that is currently attacking
  // @param creature_2 creature that is currently being attacked
  // @param first_strike bool that indicates if the first strike trait is active
  // @param continue_battle determines if a second attack should take place
  // @param attacking_creature_owner Owner of the current attacking creature
  //
  // @return returns true if no end condition is reached
  //
  bool startAttack(Creature*& creature_1, Creature*& creature_2, bool first_strike, bool& continue_battle,
                   Player*& attacking_creature_owner);

  //------------------------------------------------------------------------------------------------------------------
  // moves the creatures that died during a single battle (one slot) to the graveyard
  //
  // @param creature_1 first creature pointer
  // @param creature_2 second creature pointer
  // @param first_attacker_role Role of the Player which creature attacked first
  // @param column column where the fight takes place
  //
  void removeDeadCreatures(Creature*& creature_1, Creature*& creature_2, Role& first_attacker_role, int column);

  //------------------------------------------------------------------------------------------------------------------
  // starts a fight between two creatures and processes it
  //
  // @param column column where the fight takes place
  //
  // @return returns true if no end condition is reached
  //
  bool creatureFight(int column);

  //------------------------------------------------------------------------------------------------------------------
  // processes a certain specified battle trait
  //
  // @param creature_1 creature that is currently attacking
  // @param creature_2 creature that is currently being attacked
  // @param trait bool to a trait that is being checked
  // @param trait_name trait to be checked as a string
  //
  // @return returns role of the first attacker
  //
  Role processBattleTraits(Creature*& creature_1, Creature*& creature_2, bool& trait, std::string trait_name);

  //------------------------------------------------------------------------------------------------------------------
  // processes all traits in between turns (during the setup phase)
  //
  void processTurnTraits();

  //------------------------------------------------------------------------------------------------------------------
  // starts a battle at a certain column
  //
  // @param column column where the attack takes place
  //
  // @return returns role of the first attacker
  //
  bool startBattle(int column);

  //------------------------------------------------------------------------------------------------------------------
  // processes a command and executes it depending on the type
  //
  // @param command the command to be processed
  //
  void processCommand(Command command);

  //------------------------------------------------------------------------------------------------------------------
  // getter for the current round
  //
  // @return returns the current round
  //
  int getRound() const;

  //------------------------------------------------------------------------------------------------------------------
  // prints the game board (all player zones)
  //
  void printBoard() const;

  //------------------------------------------------------------------------------------------------------------------
  // begins the final phase of the game, determining a winner and printing the corresponding message
  //
  // @param file_path the path of the config file  thatt needs to be appended with the winner
  //
  // @return true if the file was editable
  //         false if it wasn't
  // 
  bool startFinalPhase(std::string file_path);

  //------------------------------------------------------------------------------------------------------------------
  // returns a creature that is occupying a specific position
  //
  // @param field_string a string with the format: O?(F|B)[1-7]
  //                     O signaling if it target's the opponent's board or own board
  //                     F or B signaling Field zone or Battlle zone
  //                     1-7 signaling the column of the zone
  //
  // @return the creature pointer that is occupying the given field position
  //
  Creature* getCreatureFromInputString(std::string field_string);

  //------------------------------------------------------------------------------------------------------------------
  // calculates the mana cost of spells with X mana value
  //
  // @param spell_card a spell pointer to the card that is being cast
  // @param second_param a string of a field slot or card id
  // 
  // @return the true mana value
  //
  int calculateXMana(Spell* spell_card, std::string second_param);

  //------------------------------------------------------------------------------------------------------------------
  // target player's creature takes an amount of damage, and is put into the graveyard if it reaches 0 or less
  // health
  //
  // @param amount an integer dictating how much damage is taken
  // @param target_player a player pointer pointing to the affected player
  // @param zone_type a char 'B' or 'F' for battle  zone or field zone
  // @param column an integer dictating the column of the zone in which the affected creature is located
  // 
  void creatureTakesDamage(int amount, Player* target_player, char zone_type, int column);

  //------------------------------------------------------------------------------------------------------------------
  // target player's creature takes an amount of damage, and is put into the graveyard if it reaches 0 or less
  // health
  //
  // @param amount an integer dictating how much damage is taken
  // @param creature a creature pointer pointing to the creature that is damaged
  // @param owner a player pointer pointing to the player whose creature is affected
  // 
  void creatureTakesDamage(int amount, Creature* creature, Player* owner);


  ///COMMANDS

  //------------------------------------------------------------------------------------------------------------------
  // executes the help command, printing all possible command options
  // 
  void printHelp() const;

  //------------------------------------------------------------------------------------------------------------------
  // executes the info command, printing information about a specific card
  //
  // @param command a refrence to the Commmand class to refer to the user's command and parameters input
  // 
  void printInfo(Command& command) const;

  //------------------------------------------------------------------------------------------------------------------
  // executes the redraw command
  // 
  void processRedraw();

  //------------------------------------------------------------------------------------------------------------------
  // executes the status command, printing general information about the game
  // 
  void printStatus() const;

  //------------------------------------------------------------------------------------------------------------------
  // switches the board_active_ bool to true if its false, or vice versa
  // 
  void changeBoardStatus();

  //------------------------------------------------------------------------------------------------------------------
  // executes the battle command
  //
  // @param command a refrence to the Commmand class to refer to the user's command and parameters input
  // 
  void processBattle(Command& command);
  
  //------------------------------------------------------------------------------------------------------------------
  // executes the creature command, putting a creature on the plaayer's field zone
  //
  // @param command a refrence to the Commmand class to refer to the user's command and parameters input
  // 
  void processCreature(Command& command) const;

  //------------------------------------------------------------------------------------------------------------------
  // handles the errors for the spell command and calls execute spell when none are encountered
  //
  // @param command a refrence to the Commmand class to refer to the user's command and parameters input
  //
  void processSpell(Command& command);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell command and calls the specific method for the needed spell
  //
  // @param command a refrence to the Commmand class to refer to the user's command and parameters input
  // @param card the card pointer that should be deleted
  // @param mana_cost an integer that dictates how much mana should be reduced from the player
  //
  void executeSpell(Command& command, Card* card, int mana_cost);


  ///SPELLS

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Battle Cry
  // 
  void executeBattleCry();

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Meteor
  // 
  void executeMeteor();

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Fireball
  // 
  void executeFireball();

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Meteor
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeClone(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Death Curse
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeDeathCurse(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Shock
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeShock(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Mobilize
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeMobilize(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Rapid Rush
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeRapidRush(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Shield
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeShield(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Amputate
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeAmputate(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Final Act
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeFinalAct(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Loyalty
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeLoyalty(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Zombify
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeZombify(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Bloodlust
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeBloodlust(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Heroic Memory
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeHeroicMemory(std::string target);

  //------------------------------------------------------------------------------------------------------------------
  // executes the spell Revive
  // 
  // @param target a string taken from the user's input with the format: O?(F|B)[1-7]
  //                O signaling if it target's the opponent's board or own board
  //                F or B signaling Field zone or Battlle zone
  //                1-7 signaling the column of the zone
  //
  void executeRevive(std::string target);

  //NEW a3 method:
  void letAiPlay();

  void getCreaturePriority(int& priority, std::vector<std::string>& command);

  void getSpellPriority(int& priority, std::vector<std::string>& command);

  void getGeneralSpellPriority(Spell* spell, int& priority, std::vector<std::string>& command);

  void getTargetSpellPriority(Spell* spell, int& priority, std::vector<std::string>& command);

  void getGraveyardSpellPriority(Spell* spell, int& priority, std::vector<std::string>& command);

  void getBattlePriority(int& priority, std::vector<std::string>& command);

  int determineBestDefenseSlot(Creature* attacking_creature, auto& undefended_creatures, int min_health_opposer);

  void printAiCommand(std::vector<std::string>& command);
};

#endif //GAME_HPP
