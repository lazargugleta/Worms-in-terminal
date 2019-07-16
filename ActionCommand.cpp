//------------------------------------------------------------------------------
// ActionCommand.cpp
//------------------------------------------------------------------------------
//

#include "ActionCommand.h"
#include "Worm.h"
#include <utility>
#include <iostream>
#include <sstream>
using Sep::Action;
using Sep::Command;
using Sep::Game;
using Sep::Worm;
using std::string;

namespace Sep
{
  const string ACTION_STRING = "action";
  const string IDLE_STRING = "idle";
  const string WRONG_PARAMETER_COUNT = "[ERROR] wrong parameter count!\n";
  const string INVALID_PARAMETER = "[ERROR] invalid parameter!\n";

  const string LEFT_STRING = "l";
  const string RIGHT_STRING = "r";
  const string UP_STRING = "u";
  const string DOWN_STRING = "d";
  const string RIGHT_DOWN_STRING = "rd";
  const string RIGHT_UP_STRING = "ru";
  const string LEFT_DOWN_STRING = "ld";
  const string LEFT_UP_STRING = "lu";
  const int RETURN_ERROR = -1;
  const int RETURN_OK = 0;
  const int RETURN_GAME_OVER = 1;
  const int RETURN_MEMORY_ERROR = -2;
}



//------------------------------------------------------------------------------
Action::Action(std::string name) : Command(name)
{
  name_ = name;
}


//------------------------------------------------------------------------------
Action::~Action()
{
}


//------------------------------------------------------------------------------
int Action::execute(Sep::Game &game, std::vector <std::string> &params)
{
  if (params[0] != Sep::ACTION_STRING)
  {
    return Sep::RETURN_ERROR;
  }
  if ((params[1] == Sep::IDLE_STRING) && (!params[2].size()))
  {
  }
  else if (game.getWeaponOfWormOnMove() == Worm::MELEE)
  {
    if (params[1].size())
    {
      std::cout << Sep::WRONG_PARAMETER_COUNT;
      return Sep::RETURN_ERROR;
    }
    game.melee();
  }
  else if (game.getWeaponOfWormOnMove() == Worm::TELEPORT)
  {
    if ((!params[1].size()) || (!params[2].size()) || (params[3].size()))
    {
      std::cout << Sep::WRONG_PARAMETER_COUNT;
      return Sep::RETURN_ERROR;
    }
    int row;
    int col;
    std::string test_for_extra_chars = "";
    std::stringstream row_sstream(params[1]);
    std::stringstream col_sstream(params[2]);
    row_sstream >> row;
    col_sstream >> col;
    if ((row_sstream.fail()) || (col_sstream.fail()))
    {
      std::cout << Sep::INVALID_PARAMETER;
      return Sep::RETURN_ERROR;
    }
    row_sstream >> test_for_extra_chars;
    col_sstream >> test_for_extra_chars;
    if (test_for_extra_chars.size())
    {
      std::cout << Sep::INVALID_PARAMETER;
      return Sep::RETURN_ERROR;
    }
    if (game.teleport(row, col) == Sep::RETURN_ERROR)
      return Sep::RETURN_ERROR;
  }
  else if (game.getWeaponOfWormOnMove() == Worm::AIRSTRIKE)
  {
    if ((!params[1].size()) || params[2].size())
    {
      std::cout << Sep::WRONG_PARAMETER_COUNT;
      return Sep::RETURN_ERROR;
    }
    int column;
    std::stringstream arguments;
    arguments.str("");
    arguments << params[1];
    arguments >> column;
    arguments.str("");
    if (arguments.fail())
    {
      std::cout << Sep::INVALID_PARAMETER;
      return Sep::RETURN_ERROR;
    }
    if (game.airstrike(column) == Sep::RETURN_ERROR)
      return Sep::RETURN_ERROR;
  }
  else
  {
    if ((!params[1].size()) || params[2].size())
    {
      std::cout << Sep::WRONG_PARAMETER_COUNT;
      return Sep::RETURN_ERROR;
    }
    std::pair<int, int> direction_change = directionSwitch(params[1]);
    if ((!direction_change.first) && (!direction_change.second))
    {
      std::cout << Sep::INVALID_PARAMETER;
      return Sep::RETURN_ERROR;
    }
    if (game.getWeaponOfWormOnMove() == Worm::GUN)
      game.gun(direction_change);
    else if (game.getWeaponOfWormOnMove() == Worm::BAZOOKA)
      game.bazooka(direction_change);
    else // game
      game.blowtorch(direction_change);
  }
  if (game.victoryCheck())
    return Sep::RETURN_GAME_OVER;
  game.resetWeaponChoice();
  game.increaseTurn();
  if (game.addChest())
    return Sep::RETURN_MEMORY_ERROR;
  game.printMap();
  game.printOnMove();
  return Sep::RETURN_OK;
}

std::pair<int, int> Action::directionSwitch(std::string direction)
{
  std::pair<int, int> direction_change;
  if (direction == Sep::LEFT_STRING)
  {
    direction_change.first = 0;
    direction_change.second = -1;
  }
  else if (direction == Sep::RIGHT_STRING)
  {
    direction_change.first = 0;
    direction_change.second = 1;
  }
  else if (direction == Sep::UP_STRING)
  {
    direction_change.first = -1;
    direction_change.second = 0;
  }
  else if (direction== Sep::DOWN_STRING)
  {
    direction_change.first = 1;
    direction_change.second = 0;
  }
  else if (direction == Sep::LEFT_DOWN_STRING)
  {
    direction_change.first = 1;
    direction_change.second = -1;
  }
  else if (direction == Sep::RIGHT_DOWN_STRING)
  {
    direction_change.first = 1;
    direction_change.second = 1;
  }
  else if (direction == Sep::LEFT_UP_STRING)
  {
    direction_change.first = -1;
    direction_change.second = -1;
  }
  else if (direction == Sep::RIGHT_UP_STRING)
  {
    direction_change.first = -1;
    direction_change.second = 1;
  }
  else
    direction_change.first = direction_change.second = 0;
  return direction_change;
}
