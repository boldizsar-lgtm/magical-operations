#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Command.hpp"

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
  Game() = default;
  Game(Game&) = default;
  ~Game() = default;

  //Methods
  void startGame();
  void startRound();
  void startSetupPhase();
  void startBattlePhase();
  void printBoard();
  void processCommand(Command command);
};

#endif //GAME_HPP
