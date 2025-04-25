#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Config.hpp"
//#include "CommandParse.hpp"

class Game
{
protected:
  Player player1_;
  Player player2_;
  Player* attacking_player_;
  Player* defending_player_;
  int max_rounds_;
  int round_count_;
  Config config_;
  //CommandParse command_parser_;

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
};

#endif //GAME_HPP
