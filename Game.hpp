#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <regex>
#include "Config.hpp"
#include "Player.hpp"
#include "Command.hpp"
#include "CommandError.hpp"
#include "CommandLine.hpp"

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

public:
  // Constructor, Copy-Assignment-Operator, Destructor
  Game(int max_rounds_);
  Game(Game&) = default;
  ~Game() = default;

  // Methods
  bool startRound();
  bool startSetupPhase();
  bool doCommandLoop(CommandLine& cli);
  bool startBattlePhase();
  bool startBattle();
  bool startRoundEnd();
  void processCommand(Command command);
  int getRound() const;
  void printBoard() const;
  //bool checkEndConditions();

  // The different commands
  void printHelp() const;
  void printInfo(Command& command) const;
  void processRedraw(Command& command);
  void printStatus() const;
  void changeBoardStatus();
  void processBattle(Command& command);
  void processCreature(Command& command) const;
  void processSpell(Command& command);
  void executeSpell(Command& command);

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
