//------------------------------------------------------------------------------
// Worm.cpp
//------------------------------------------------------------------------------
//

#include "Worm.h"

using Sep::Worm;
using std::string;

namespace Sep
{
  const int DROWNED = -1;
  const int FALL_DAMAGE = 0;
  const int OUT_OF_MAP = 2;
  const string LEFT_BRACKET = " (";
  const string WORM_DROWNED_MESSAGE = ") drowned.\n";
  const string WORM_FELL_INTO_DEATH_MESSAGE = ") fell into his death.\n";
  const string WORM_FELL_OUT_OF_MAP_MESSAGE = ") fell out of the map.\n";
  const string WORM_DIED_MESSAGE = ") died.\n";
}
using Sep::Worm;

//------------------------------------------------------------------------------
Worm::Worm() : health_(100), selected_weapon_(Worm::GUN)
{
  ammo_[Worm::BAZOOKA] = 1;
  ammo_[Worm::TELEPORT] = 1;
  ammo_[Worm::BLOWTORCH] = 1;
  ammo_[Worm::MELEE] = 1;
  ammo_[Worm::AIRSTRIKE] = 1;
}


//------------------------------------------------------------------------------
void Worm::selectWeapon(WeaponType type)
{
  selected_weapon_ = type;
}

//------------------------------------------------------------------------------
Worm::WeaponType Worm::getWeapon()
{
  return selected_weapon_;
}


//------------------------------------------------------------------------------
void Worm::removeAmmo(WeaponType type)
{
  ammo_[type] -= 1;
}

//------------------------------------------------------------------------------
void Worm::addAmmo(WeaponType type)
{
  ammo_[type] += 1;
}

//------------------------------------------------------------------------------
int Worm::getAmmo(WeaponType type)
{
  return ammo_[type];
}


//------------------------------------------------------------------------------
void Worm::setRow(int row)
{
  row_ = row;
}

//------------------------------------------------------------------------------
void Worm::setColumn(int col)
{
  col_ = col;
}

//------------------------------------------------------------------------------
void Worm::setCoordinates(int row, int col)
{
  col_ = col;
  row_ = row;
}

//------------------------------------------------------------------------------
void Worm::setHealth(int health)
{
  health_ = health;
}


//------------------------------------------------------------------------------
void Worm::setId(int id)
{
  worm_id_ = id;
}

//------------------------------------------------------------------------------
void Worm::setName(std::string name)
{
  name_ = name;
}

//------------------------------------------------------------------------------
void Worm::removeHealth(int damage)
{
  health_ -= damage;
}

//------------------------------------------------------------------------------
int Worm::getColumn()
{
  return col_;
}

//------------------------------------------------------------------------------
int Worm::getRow()
{
  return row_;
}

//------------------------------------------------------------------------------
int Worm::getHealth()
{
  return health_;
}

//------------------------------------------------------------------------------
std::string Worm::getName()
{
  return name_;
}

//------------------------------------------------------------------------------
int Worm::getId()
{
  return worm_id_;
}

//------------------------------------------------------------------------------
void Worm::death(int cause)
{
  if (cause == DROWNED)
  {
    std::cout << getName() << Sep::LEFT_BRACKET << getId() << 
          Sep::WORM_DROWNED_MESSAGE;
  }
  else if (cause == FALL_DAMAGE)
  {
    std::cout << getName() << Sep::LEFT_BRACKET << getId() << 
          Sep::WORM_FELL_INTO_DEATH_MESSAGE;
  }
  else if (cause == OUT_OF_MAP)
  {
    std::cout << getName() << Sep::LEFT_BRACKET << getId() << 
          Sep::WORM_FELL_OUT_OF_MAP_MESSAGE;
  }
  else
  {
    std::cout << getName() << Sep::LEFT_BRACKET << getId() << 
          Sep::WORM_DIED_MESSAGE;
  }
}

