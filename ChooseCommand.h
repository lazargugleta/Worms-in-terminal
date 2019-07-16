//------------------------------------------------------------------------------
// ChooseCommand.h
//
// Declarations for choose command class
// 
//------------------------------------------------------------------------------
//

#ifndef CHOOSE_H_INCLUDED
#define CHOOSE_H_INCLUDED

#include "Command.h"
#include <iostream>
#include "Game.h"
#include <string>
#include "Worm.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  // Choosecommand Class
  // Part of command pattern, Class derived from Command, used to execute 
  // choose / to select wanted weapon for worm on turn
  // 
  //
  class Choose : public Command
  {
    private:
    //------------------------------------------------------------------------
    // Name of this command
    std::string choose_name_;
    
    public:
    //------------------------------------------------------------------------
    // Executes the command.
    // @param game The game where action should be performed on
    // @param params Possible parameters needed for the execution
    // @return Integer representing the success of the action
    //
    int execute(Game &game, std::vector <std::string> &params) override;
  
    //------------------------------------------------------------------------
    // Constructor
    //
    Choose(std::string name);

    //------------------------------------------------------------------------
    // Destructor
    //
    ~Choose();
  };
}



#endif // CHOOSE_H_INCLUDED
