//------------------------------------------------------------------------------
// MoveCommand.cpp
// 
//------------------------------------------------------------------------------
//

#include "MoveCommand.h"
#include <iostream>


using Sep::MoveCommand;
using Sep::Game;
using Sep::Command;
using std::string;

namespace Sep
{
  const string MOVE_STRING = "move";
  const string WRONG_PARAMETER_COUNT = "[ERROR] wrong parameter count!\n";
  const string INVALID_PARAMETER = "[ERROR] invalid parameter!\n";
  const string LEFT_STRING = "l";
  const string RIGHT_STRING = "r";
  const int RETURN_ERROR = -1;
  const int RETURN_GAME_OVER = 1;
  const int RETURN_OK = 0;
  const int ALREADY_MOVED = 1;
}


//------------------------------------------------------------------------------
MoveCommand::MoveCommand(std::string name) : Command(name)
{
  move_name_ = name;
}


//------------------------------------------------------------------------------
MoveCommand::~MoveCommand()
{
}


//------------------------------------------------------------------------------
int MoveCommand::execute(Game &game, std::vector <std::string> &params)
{
  if (params[0] != Sep::MOVE_STRING)
  {
    return Sep::RETURN_ERROR;
  }
  if ((params[3].size()) || (!params[2].size()) || (!params[1].size()))
  {
    std::cout << Sep::WRONG_PARAMETER_COUNT;
    return Sep::RETURN_ERROR;
  }
  if ((params[1] != Sep::LEFT_STRING) && (params[1] != Sep::RIGHT_STRING))
  {
    std::cout << Sep::INVALID_PARAMETER;
    return Sep::RETURN_ERROR;
  }
  std::stringstream third_word(params[2]);
  int steps;
  third_word >> steps;
  if ((third_word.fail()) || (steps < 0) || (steps > 3))
  {
    third_word.clear();
    std::cout << Sep::INVALID_PARAMETER;
    return Sep::RETURN_ERROR;
  }
  if (params[1] == Sep::LEFT_STRING)
    steps *= -1;
  int result_of_move = game.moveLogic(steps);
  if (result_of_move == Sep::ALREADY_MOVED)
    return Sep::RETURN_OK;
  game.printMap();
  if (result_of_move == Sep::RETURN_ERROR)
  {
    if (game.victoryCheck())
      return Sep::RETURN_GAME_OVER;
    game.increaseTurn();
    game.addChest();
    game.printMap();
    game.printOnMove();
  }
  return Sep::RETURN_OK;
  
}