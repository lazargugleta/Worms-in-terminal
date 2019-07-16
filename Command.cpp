//------------------------------------------------------------------------------
// Filename: Command.cpp
// Description: Class representing a general command
//
//------------------------------------------------------------------------------

#include "Command.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  Command::Command(std::string name) : command_name_(name)
  {
  }
  
  //----------------------------------------------------------------------------
  Command::~Command()
  {
  }
  
  //----------------------------------------------------------------------------
  const std::string &Command::getName() const
  {
    return command_name_;
  }
}