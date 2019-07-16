//------------------------------------------------------------------------------
// SingleWordCommand.h
//------------------------------------------------------------------------------
//

#ifndef SINGLEWORDCOMMAND_H
#define SINGLEWORDCOMMAND_H



#include "Command.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  // SingleWordCommand Class
  // Part of command pattern, Class derived from Command, used to execute 
  // single word commands like map, help, state, quit
  // 
  //
  class SingleWordCommand : public Command
  {
    private:
    //--------------------------------------------------------------------------
    // Name of this command
    std::string single_word_name_;
  
    //--------------------------------------------------------------------------
    // prints help
    //
    void help();
    
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
    SingleWordCommand(std::string name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    ~SingleWordCommand();
  };
}

#endif //SINGLEWORDCOMMAND_H
