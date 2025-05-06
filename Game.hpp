#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "Player.hpp"
#include "Command.hpp"
#include "Config.hpp"
#include "CommandError.hpp"
#include "CommandLine.hpp"

class Game
{
protected:
  Player player1_;
  Player player2_;
  Player* current_player_;
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
  void processCommand(Command command);
  int getRound() const;
  void printBoard() const;
  //bool checkEndConditions();

  // The different commands
  void printHelp() const;
  void printInfo(Command& command) const;
  void printStatus() const;
  void changeBoardStatus();
};

#endif //GAME_HPP
