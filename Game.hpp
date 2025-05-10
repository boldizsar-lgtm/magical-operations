#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <regex>
#include "Config.hpp"
#include "Player.hpp"
#include "Command.hpp"
#include "CommandError.hpp"
#include "CommandLine.hpp"


enum class EndCondition
{
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

  // Methods
  bool startRound();
  bool startSetupPhase();
  bool doCommandLoop(CommandLine& cli);
  bool startBattlePhase();
  void undeadCreaturesReenter(Player* player);
  void removeTemporaryCreatures(Player* player);
  void returnCreaturesToField();
  bool directHitBattle(int column);
  bool creatureFight(int column);
  void checkBattleTraits(Creature*& creature_1, Creature*& creature_2, bool& trait, std::string trait_name,
  Role& first_attacker_role);
  bool startBattle(int column);
  bool startRoundEnd();
  void processCommand(Command command);
  int getRound() const;
  void printBoard() const;
  bool startFinalPhase(std::string file_path);
  //bool checkEndConditions();

  // The different commands
  void printHelp() const;
  void printInfo(Command& command) const;
  void processRedraw();
  void printStatus() const;
  void changeBoardStatus();
  void processBattle(Command& command);
  void processCreature(Command& command) const;
  void processSpell(Command& command);
  void executeSpell(Command& command);
  void creatureTakesDamage(int  amount, Player* target_player, char zone_type, int column);

  //the differentt spells
  void executeBattleCry();
  void executeMeteor();
  void executeFireball();
  void executeClone(std::string target);
  void executeDeathCurse(std::string target);
  void executeShock(std::string target);
  void executeMobilize(std::string target);
  void executeRapidRush(std::string target);
  void executeShield(std::string target);
  void executeAmputate(std::string target);
  void executeFinalAct(std::string target);
  void executeLoyalty(std::string target);
  void executeZombify(std::string target);
  void executeBloodlust(std::string target);
  void executeHeroicMemory(std::string target);
  void executeRevive(std::string target);
};

#endif //GAME_HPP
