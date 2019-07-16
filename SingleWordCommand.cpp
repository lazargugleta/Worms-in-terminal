//------------------------------------------------------------------------------
// SingleWordCommand.cpp
// 
//------------------------------------------------------------------------------
//

#include "SingleWordCommand.h"
#include "Game.h"
using Sep::SingleWordCommand;
using Sep::Game;
using std::string;

namespace Sep
{
  const string MAP_STRING = "map";
  const string HELP_STRING = "help";
  const string STATE_STRING = "state";
  const string QUIT_STRING = "quit";
  const string ERROR_UNKNOWN_COMMAND = "[ERROR] unknown command!\n";
  const string ERROR_WRONG_PARAM_COUNT = "[ERROR] wrong parameter count!\n";
  const int RETURN_ERROR = -1;
  const int RETURN_OK = 0;
  const int RETURN_GAME_OVER = 1;
  
  const string AVAILIBLE_COMMANDS = "Available Commands:\n";
  const string MOVE_USAGE = "  move [left/right] [0..3]\n    Move your worm.\n";
  const string CHOOSE_USAGE = "  choose [weapon]\n    Select your weapon\n";
  const string ACTION_USAGE = 
        "  action [l/r/d/u/ld/rd/lu/ru]\n    Shoots current weapon.\n";
  const string ACTIVATES_BLOWTORCH = "    Activates blowtorch.\n";
  const string MELEE_ACTION = "  action\n    Activates melee attack.\n";
  const string ACTION_TELEPORT = 
        "  action [row] [col]\n    Teleports to coordinate.\n";
  const string ACTION_AIRSTRIKE = "  action [col]\n    Airstrike.\n";
  const string ACTION_IDLE = "  action idle\n    Do nothing.\n";
  const string QUIT_USAGE = "  quit\n    End the game.\n";
  const string HELP_USAGE = "  help\n    Display this help.\n";
  const string STATE_USAGE = "  state\n    Print the current game state.\n";
}


//------------------------------------------------------------------------------
SingleWordCommand::SingleWordCommand(std::string name) : Command(name)
{
  single_word_name_ = name;
}


//------------------------------------------------------------------------------
SingleWordCommand::~SingleWordCommand()
{
}


//------------------------------------------------------------------------------
int SingleWordCommand::execute(Game &game, std::vector <std::string> &params)
{
  if ((params[0] != Sep::MAP_STRING) && (params[0] != Sep::HELP_STRING) && 
        (params[0] != Sep::STATE_STRING) && (params[0] != Sep::QUIT_STRING))
  {
    std::cout << Sep::ERROR_UNKNOWN_COMMAND;
    return Sep::RETURN_ERROR;
  }
  
  if (params[1].size())
  {
    std::cout << Sep::ERROR_WRONG_PARAM_COUNT;
    return Sep::RETURN_ERROR;
  }
  
  if (params[0] == Sep::MAP_STRING)
    game.printMap();
  else if (params[0] == Sep::HELP_STRING)
    help();
  else if (params[0] == Sep::STATE_STRING)
    game.state();
  else if (params[0] == Sep::QUIT_STRING)
    return Sep::RETURN_GAME_OVER;
  
  return Sep::RETURN_OK;
  
}


//------------------------------------------------------------------------------
void SingleWordCommand::help()
{
  std::cout << Sep::AVAILIBLE_COMMANDS << Sep::MOVE_USAGE << Sep::CHOOSE_USAGE;
  std::cout << Sep::ACTION_USAGE << Sep::ACTIVATES_BLOWTORCH;
  std::cout << Sep::MELEE_ACTION << Sep::ACTION_TELEPORT;
  std::cout << Sep::ACTION_AIRSTRIKE << Sep::ACTION_IDLE;
  std::cout << Sep::QUIT_USAGE;
  std::cout << Sep::HELP_USAGE;
  std::cout << Sep::STATE_USAGE;
  std::cout.flush();
}
