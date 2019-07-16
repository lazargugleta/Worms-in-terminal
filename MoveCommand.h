//------------------------------------------------------------------------------
// MoveCommand.h
//
// Declarations for MoveCommand class
// 
//------------------------------------------------------------------------------
//

#ifndef MOVECOMMAND_H_INCLUDED
#define MOVECOMMAND_H_INCLUDED

#include <sstream>
#include <string>
#include "Command.h"
#include "Game.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  // Movecommand Class
  // Part of command pattern, Class derived from Command, used to execute 
  // move on the worm that is on turn
  // 
  //
  class MoveCommand : public Command
  {
    private:
    //------------------------------------------------------------------------
    // Name of this command
    std::string move_name_;
    
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
    MoveCommand(std::string name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    ~MoveCommand();
    
  };
}

#endif // MOVECOMMAND_H_INCLUDED