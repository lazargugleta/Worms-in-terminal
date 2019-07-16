//------------------------------------------------------------------------------
// ActionCommand.h
//
// Declartaions for ActionCommand class.
// 
//------------------------------------------------------------------------------
//


#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include "Command.h"
#include "Game.h"
#include <sstream>


namespace Sep
{

  //----------------------------------------------------------------------------
  // Actioncommand Class
  // Part of command pattern, Class derived from Command, used to execute 
  // action / to fire selected weapon from worm on move
  // 
  //
  class Action : public Command
  {
    private:
    //--------------------------------------------------------------------------
    // Name of this command
    std::string name_;


    //--------------------------------------------------------------------------
    //helper direction swithc method thakes string of 1 or 2 letters-direction
    // param and turns it into a number on corresponding oon the numpad to
    // wanted direction
    //
    std::pair<int, int> directionSwitch(std::string direction);


    public:
    //--------------------------------------------------------------------------
    // Executes the command.
    // @param game The game where action should be performed on
    // @param params Possible parameters needed for the execution
    // @return Integer representing the success of the action
    //
    int execute(Game &game, std::vector <std::string> &params) override;

    //--------------------------------------------------------------------------
    // Constructor
    //
    Action(std::string name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    ~Action();
  };
  
  
}

#endif //ACTION_H_INCLUDED
