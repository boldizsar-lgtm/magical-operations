#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "Player.hpp"
#include "Command.hpp"
#include "Config.hpp"
#include "CommandError.hpp"

class Game
{
protected:
  Player player1_;
  Player player2_;
  Player* attacking_player_;
  Player* defending_player_;
  int max_rounds_;
  int round_count_;
  bool board_active_;

public:
  //Constructor, Copy-Assignment-Operator, Destructor
  Game(int max_rounds_);
  Game(Game&) = default;
  ~Game() = default;

  //Methods
  void startGame();
  void startRound();
  void startSetupPhase();
  void startBattlePhase();
  void printBoard();
  void processCommand(Command command);
  int getRound() const;
};

#endif //GAME_HPP
