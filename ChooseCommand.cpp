//------------------------------------------------------------------------------
// ChooseCommand.cpp
// 
//------------------------------------------------------------------------------
//

#include "ChooseCommand.h"

using Sep::Game;
using Sep::Choose;
using Sep::Worm;
using Sep::Command;
using std::string;

namespace Sep
{
  const string CHOOSE_STRING = "choose";
  const string GUN_STRING = "gun";
  const string BAZOOKA_STRING = "bazooka";
  const string TELEPORT_STRING = "teleport";
  const string MELEE_STRING = "melee";
  const string BLOWTORCH_STRING = "blowtorch";
  const string AIRSTRIKE_STRING = "airstrike";
  const string ERROR_NO_AMMO = "[ERROR] no ammunition\n";
  const string WRONG_PARAMETER_COUNT = "[ERROR] wrong parameter count!\n";
  const string INVALID_PARAMETER = "[ERROR] invalid parameter!\n";
  const string CHOSE_WEAPON = "Chose weapon ";
  const string AMUNITION = " Ammunition: ";
  const string INFINITE = "inf\n";
  const int RETURN_ERROR = -1;
  const int RETURN_OK = 0;
}

//------------------------------------------------------------------------------
Choose::Choose(std::string name) : Command(name)
{
  choose_name_ = name;
}


//------------------------------------------------------------------------------
Choose::~Choose()
{
}


//------------------------------------------------------------------------------
int Choose::execute(Game &game, std::vector<std::string> &params)
{
  if (params[0] != Sep::CHOOSE_STRING)
  {
    return Sep::RETURN_ERROR;
  }
  if ((params[2].size()) || (!params[1].size()))
  {
    std::cout << Sep::WRONG_PARAMETER_COUNT;
    return Sep::RETURN_ERROR;
  }
  //check if passed argument is any of the valid strings
  if (params[1] == Sep::GUN_STRING)
  {
    game.selectWeaponForWormOnMove(Worm::GUN);
  }
  else if (params[1] == Sep::BAZOOKA_STRING)
  {    //these will return non-0  only if there is no ammo
    if (game.selectWeaponForWormOnMove(Worm::BAZOOKA))
    {
      std::cout << Sep::ERROR_NO_AMMO;
      return Sep::RETURN_OK;
    }
  }
  else if (params[1] == Sep::TELEPORT_STRING)
  {
    if (game.selectWeaponForWormOnMove(Worm::TELEPORT))
    {
      std::cout << Sep::ERROR_NO_AMMO;
      return Sep::RETURN_OK;
    }
  }
  else if (params[1] == Sep::BLOWTORCH_STRING)
  {
    if (game.selectWeaponForWormOnMove(Worm::BLOWTORCH))
    {
      std::cout << Sep::ERROR_NO_AMMO;
      return Sep::RETURN_OK;
    }
  }
  else if (params[1] == Sep::MELEE_STRING)
  {
    if (game.selectWeaponForWormOnMove(Worm::MELEE))
    {
      std::cout << Sep::ERROR_NO_AMMO;
      return Sep::RETURN_OK;
    }
  }
  else if (params[1] == Sep::AIRSTRIKE_STRING)
  {
    if (game.selectWeaponForWormOnMove(Worm::AIRSTRIKE))
    {
      std::cout << Sep::ERROR_NO_AMMO;
      return Sep::RETURN_OK;
    }
  }
  else
  {
    std::cout << Sep::INVALID_PARAMETER;
    return Sep::RETURN_ERROR;
  }
  std::string selected_weapon;
  selected_weapon = game.gainedWeaponSwitch(game.getWeaponOfWormOnMove());
  std::cout << Sep::CHOSE_WEAPON <<  selected_weapon << Sep::AMUNITION;
  
  if (selected_weapon== Sep::GUN_STRING)
    std::cout << Sep::INFINITE;
  else
    std::cout << game.selectedAmmoOfWormOnMove() << std::endl;
  return Sep::RETURN_OK;
}