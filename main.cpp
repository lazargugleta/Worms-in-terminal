//------------------------------------------------------------------------------
// Filename: main.cpp
//
// Description: simple main
//------------------------------------------------------------------------------

#include "Random.h"
#include "Game.h"
#include "Command.h"
#include "MoveCommand.h"
#include "ActionCommand.h"
#include "SingleWordCommand.h"
#include "ChooseCommand.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>



namespace Sep
{
  const int RETURN_OK = 0;
  const int RETURN_ERROR_BAD_CONFIG = -1;
  const int GAME_OVER = 1;
  const int RETURN_ERROR_MEMORY = -2;
  const int RETURN_ERROR_ARGUMENT = -3;
  const std::string ERROR_WRONG_ARGUMENTS = "Usage: ./sworms <config-file>";
  const std::string SEP_PROMPT =  "sep> ";
  const std::string CHOOSE_STRING = "choose";
  const std::string MOVE_STRING = "move";
  const std::string ACTION_STRING = "action";
  const std::string ERROR_BAD_MEMORY = "[ERROR] memory error!\n";
}

//Forwaard declaration
int readUserInput(Sep::Game &game);

//------------------------------------------------------------------------------
// The main program.
// Used to create a Game object, to prepare it for playing the game by 
// loading the map, adding worms, and setting first turn, the calling the
// readUserInput function which reads user input, then executes the 
// appropriate command through command pattern
//
// @param int argc -  the number of passed arguments
// @param char* argv[] -  array of char* containing file from which 
// we want to load 
// @return int 0 if game went OK, -1 if bad config file, -2 if out of 
//       memory, and -3 if wrong number of args passed
//
int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    std::cout << Sep::ERROR_WRONG_ARGUMENTS << '\n';
    return Sep::RETURN_ERROR_ARGUMENT;
  }
  Sep::Game* game = nullptr;
  try
  {
    game = new Sep::Game;
  }
  catch (std::bad_alloc)
  {
    std::cout << Sep::ERROR_BAD_MEMORY;
    delete game;
    return Sep::RETURN_ERROR_MEMORY;
  }
  // Sep
  int result_of_load_config = game->loadConfig(argv[1]);
  if (result_of_load_config == Sep::RETURN_ERROR_BAD_CONFIG)
  {
    delete game;
    return Sep::RETURN_ERROR_BAD_CONFIG;
  }
  if (result_of_load_config == Sep::RETURN_ERROR_MEMORY)
  {
    delete game;
    return Sep::RETURN_ERROR_MEMORY;
  }  

  game->addWorms();
  if (game->victoryCheck())
  {
    delete game;
    return  Sep::RETURN_OK;
  }
  game->setFirstTurn();
  if (readUserInput(*game) == Sep::RETURN_ERROR_MEMORY)
  {
    std::cout << Sep::ERROR_BAD_MEMORY;
    delete game;
    return Sep::RETURN_ERROR_MEMORY;
  }
  delete game;
  return Sep::RETURN_OK;
}


//------------------------------------------------------------------------------
// Function for reading user input and running appropriate commands
// using command pattern.
// essentially an infinite while loop that is broken when game ends or
// when  memory error occurs.
//
// @param Game &Game -  reference to game object  that stores the state
//
// @return int 0 if game went OK, -2 if out of memory, 
//
int readUserInput(Sep::Game &game)
{
  std::string read;
  std::vector<std::string> params(4);
  std::stringstream read_line;
  
  Sep::Choose choose_command("chooser");
  Sep::MoveCommand move_command("mover");
  Sep::SingleWordCommand single_word_command("single");
  Sep::Action action_command("do_action");
  
  game.printMap();
  game.printOnMove();
  
  while (1)
  {
    for (auto &word : params) // empty the vector of strings
      word = "";

    std::cout << Sep::SEP_PROMPT;
    getline(std::cin, read);
    if (std::cin.eof())
      return  Sep::RETURN_OK;
      
    read_line.clear();
    read_line.str("");
    read_line.str(read);
    read_line >> params[0];
    if (params[0] == "")
      continue;
    read_line >> params[1] >> params[2] >> params[3];
    int result_of_execute;
    
    if (params[0] == Sep::MOVE_STRING)
    {
      if (move_command.execute(game, params) == Sep::GAME_OVER)
      {
        game.addChest();
        game.printMap();
        return Sep::RETURN_OK;
      }
    }
    else if (params[0] == Sep::CHOOSE_STRING)
      choose_command.execute(game, params);
    
    else if (params[0] == Sep::ACTION_STRING)
    {
      result_of_execute = action_command.execute(game, params);
      if (result_of_execute == Sep::GAME_OVER)
      {
        game.addChest();
        game.printMap();
        return Sep::RETURN_OK;
      }
      if (result_of_execute == Sep::RETURN_ERROR_MEMORY)
      {
        std::cout << Sep::ERROR_BAD_MEMORY;
        return Sep::RETURN_ERROR_MEMORY;
      }
    }
    else if (single_word_command.execute(game, params) == Sep::GAME_OVER)
    {
      return Sep::RETURN_OK;
    }
  }
}