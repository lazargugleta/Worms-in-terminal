//------------------------------------------------------------------------------
// Chest.cpp
// 
//------------------------------------------------------------------------------
//

#include "Chest.h"


using Sep::Chest;
using Sep::Worm;

//------------------------------------------------------------------------------
Chest::Chest(int col, int weapon) : row_(0), col_(col), weapon_(weapon)
{
}


//------------------------------------------------------------------------------
Worm::WeaponType Chest::getWeapon()
{
  switch (weapon_)
  {
    case 0:
      return Worm::BAZOOKA;
    case 1:
      return Worm::TELEPORT;
    case 2:
      return Worm::BLOWTORCH;
    case 3:
      return Worm::MELEE;
    case 4:
      return Worm::AIRSTRIKE;
    default:
      return Worm::GUN; // only here so we don't get warning with compile
  }
}

//------------------------------------------------------------------------------
int Chest::getRow()
{
  return row_;
}

//------------------------------------------------------------------------------
int Chest::getCol()
{
  return col_;
}

//------------------------------------------------------------------------------
void Chest::setCoordinates(int row, int col)
{
  row_ = row;
  col_ = col;
}

//------------------------------------------------------------------------------
void Chest::setRow(int row)
{
  row_ = row;
}

//------------------------------------------------------------------------------
void Chest::setCol(int col)
{
  col_ = col;
}

//------------------------------------------------------------------------------
Chest::~Chest()
{
}