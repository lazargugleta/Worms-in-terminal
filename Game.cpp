//------------------------------------------------------------------------------
// Game.cpp
// 
//------------------------------------------------------------------------------
//

#include "Random.h"
#include "Game.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>


using Sep::Game;
using Sep::Field;
using Sep::Worm;
using Sep::Chest;

namespace Sep
{
  const std::string ERROR_INVALID_FILE = "[ERROR] invalid config file\n";
  const std::string ERROR_INVALID_TARGET = "[ERROR] invalid target\n";
  const std::string WARNING_CANNOT_MOVE =  "[WARNING] can't move further\n";
  const std::string ERROR_INVALID_PARAMS = "[ERROR] invalid parameter!\n";
  const std::string ERROR_BAD_MEMORY = "[ERROR] memory error!\n";
  const std::string ERROR_NOT_ALLOWED = 
  "[ERROR] command currently not allowed!\n";
  const std::string SHOT_MISSED = "Shot missed...\n";
  const std::string END_MESSAGE = "END: ";
  const std::string DRAW_MESSAGE = "Draw";
  const std::string PLAYER_1_WIN_MESSAGE = "Player 1 win!\n";
  const std::string PLAYER_2_WIN_MESSAGE = "Player 2 win!\n";
  const std::string PICKED_UP_ONE_OF_MESSAGE = ") picked up 1 of ";
  const std::string LEFT_BRACKET_STRING = " (";
  const std::string CURRENT_PLAYER_MESSAGE = "current player: ";
  const std::string CUURENT_WORM_MESSAGE = "current worm: ";
  const std::string NEW_LINE = "\n";
  const std::string WEAPONS_OF_CURRENT_WORM = "weapons of current worm:\n";
  const std::string BAZOOKA_STRING = "bazooka";
  const std::string BLOWTORCH_STRING = "blowtorch";
  const std::string AIRSTRIKE_STRING = "airstrike";
  const std::string MELEE_STRING = "melee";
  const std::string TELEPORT_STRING = "teleport";
  const std::string GUN_STRING = "gun";
  const std::string GUN_AMMO_INF = "  gun (ammunition: inf)";
  const std::string AMMUNITION_MESSAGE = " (ammunition: ";
  const std::string WORMS_OF_PLAYER_ONE = "\nworms of player 1\n";
  const std::string WORMS_OF_PLAYER_TWO = "worms of player 2\n";
  const std::string HITPOINTS_MESSAGE = " HP (";
  const std::string MELEE_HIT_WORM_MESSAGE = "Attack hit Worm at position (";
  const std::string HP_DAMAGE_MESSAGE = "hp damage\n";
  const std::string TOOK_MESSAGE = ") took ";
  const std::string HIT_CHEST_AT_MESSAGE = " hit Chest at position (";
  const std::string HIT_WORM_AT_MESSAGE = " hit Worm at position (";
  const std::string HIT_EARTH_AT_MESSAGE = " hit Earth at position (";
  const std::string SHOT_STRING = "Shot";
  const std::string TORCH_STRING = "Torch";
  const std::string FALL_DAMAGE_MESSAGE =  "hp fall damage\n";
  const std::string CURRENT_MAP_MESSAGE = "Current Map:\n";
  const std::string SPAWNING_MESSAGE = "spawning ";
  const std::string AT_MESSAGE = ") at (";
  const std::string WORM_MESSAGE = " Worm ";
  const std::string PLAYER_MESSAGE = "Player ";
  const std::string READY_MESSAGE = ") ready\n";

  const int RETURN_ERROR = -1;
  const int RETURN_OK = 0;
  const int RETURN_GAME_OVER = 1;
  const int RETURN_ERROR_INVALID_CFG_FILE = -1;
  const int RETURN_ERROR_NO_WORM = -1;
  const int RETURN_ERROR_NO_CHEST = -1;
  const int RETURN_ERROR_MEMORY = -2;
  const int MAX_BOARD_SIZE = 80;
  const int MIN_BOARD_SIZE = 10;
  const int FELL_INTO_WATER = -1;
  const int FELL_TO_DEATH = 0;
  const int KILLED_BY_WEAPON = 1;
  const int FELL_OUT_OF_MAP = 2;
  const int BAZOOKA_DAMAGE = 40;
  const int GUN_DAMAGE = 25;
  const int BLOWTORCH_DAMAGE = 35;
  const int AIRSTRIKE_DAMAGE = 20;
  const int MELEE_DAMAGE = 50;
  const int NUMBER_OF_WEAPONS = 5;
  const int ALREADY_MOVED = 1;
  
}


//------------------------------------------------------------------------------
Game::Game() : board_width_(0), board_height_(0)
{
  map_.resize(MIN_BOARD_SIZE, std::vector<Field>());
}


//------------------------------------------------------------------------------
Game::~Game()
{
  for (Chest* chest : chests_)
    delete chest;
  chests_.clear();
}


//------------------------------------------------------------------------------
void Game::resetWeaponChoice()
{
  if (findWormWithId(worm_on_move_) == Sep::RETURN_ERROR_NO_WORM)
  {
    return;
  }
  int current = findWormOnMove();
  worms_[current].selectWeapon(Worm::GUN);
}



//------------------------------------------------------------------------------
int Game::victoryCheck()
{
  //draw
  if(!worms_.size())
  {
    std::cout << Sep::END_MESSAGE << Sep::DRAW_MESSAGE << std::endl;
    return Sep::RETURN_GAME_OVER;
  }
  //victory
  bool player_one_alive = false;
  bool player_two_alive = false;
  int counter;
  for (counter = 0; counter < static_cast<int>(worms_.size()); counter++)
  {
    if(worms_[counter].getId() <= 3)
    {
      player_one_alive = true;
    }
    else
    {
      player_two_alive = true;
    }
  }
  if(!player_one_alive)
  {
    std::cout << Sep::END_MESSAGE << Sep::PLAYER_2_WIN_MESSAGE;
    return Sep::RETURN_GAME_OVER;
  }
  if (!player_two_alive)
  {
    std::cout << Sep::END_MESSAGE << Sep::PLAYER_1_WIN_MESSAGE;
    return Sep::RETURN_GAME_OVER;
  }
  return Sep::RETURN_OK;
}

//------------------------------------------------------------------------------
int Game::selectWeaponForWormOnMove(Worm::WeaponType weapon)
{
  int current = findWormOnMove();
  if (weapon == Worm::GUN)
  {
    worms_[current].selectWeapon(weapon);
    return Sep::RETURN_OK;
  }
  if (!worms_[current].getAmmo(weapon))
    return Sep::RETURN_ERROR;
  worms_[current].selectWeapon(weapon);
  return Sep::RETURN_OK;
}


//------------------------------------------------------------------------------
int Game::selectedAmmoOfWormOnMove()
{
  int worm = findWormOnMove();
  return worms_[worm].getAmmo(worms_[worm].getWeapon());
}


//------------------------------------------------------------------------------
int Game::findChestAt(int row, int col)
{
  int chest_count;
  for (chest_count = 0; chest_count < static_cast<int>(chests_.size());
          chest_count++)
  {
    if ((chests_[chest_count]->getRow() == row) && 
            (chests_[chest_count]->getCol() == col))
    {
      return chest_count; // return index of found chest
    }
  }
  return Sep::RETURN_ERROR; // return -1 if no chest found
}


//------------------------------------------------------------------------------
int Game::addChest()
{
  //create two random integers
  int random_column = 
      random_number_generator_.getRandomInt(0, board_width_ - 1);
  int weapon_number = 
      random_number_generator_.getRandomInt(0, NUMBER_OF_WEAPONS - 1);
  
  if (map_[0][random_column].getType() != Field::AIR)
    return Sep::RETURN_OK; // if chest cannot be spawened here, nothing happens
  Chest* new_chest = nullptr;
  try // to create a new Chest object
  {
    new_chest = new Chest(random_column, weapon_number); 
  }
  catch (std::bad_alloc)
  {
    return Sep::RETURN_ERROR_MEMORY;
  }
  
  map_[0][random_column].setType(Field::CHEST); // put it in the map
  chests_.push_back(new_chest); // add it to our list of chests
  chestGravity(chests_.size() - 1); // and apply gravity to it
  return  Sep::RETURN_OK;
}

//------------------------------------------------------------------------------
void Game::chestGravity(int index)
{
  int row = chests_[index]->getRow(); // row and beginning_row set to current
  int beginning_row = row;           // row of wanted chest
  int col = chests_[index]->getCol();

  while (1)
  {
    row++;
    if (row >= board_height_) // if the chest fell out of the map, we delete it
    {
      delete chests_[index];
      chests_[index] = nullptr;
      chests_.erase(chests_.begin() + index);
      int above = findChestAt(beginning_row - 1, col);
      if (above != Sep::RETURN_ERROR_NO_CHEST)
        chestGravity(above);
      return;
    }
    if (map_[row][col].getType() == Field::AIR)
      continue;
    break; // we break as soon as we hit a non-air field
  }

  map_[beginning_row][col].setType(Field::AIR); // set beginning row to air

  if (map_[row][col].getType() == Field::WORM)
  {
    pickUpChest(row, col, index); // found worm picks up chest
  }
  else if (map_[row][col].getType() == Field::WATER)
  {
    delete chests_[index];
    chests_[index] = nullptr;
    chests_.erase(chests_.begin() + index); // if chest hit water, remove it
  }
  else
  {
    row--; // we hit a chest or earth, se we go back up 1 field
    map_[row][col].setType(Field::CHEST); //set our chest on the map
    chests_[index]->setRow(row); //set its internal row attribute
  }

  int above = findChestAt(beginning_row - 1, col);
  if (above != Sep::RETURN_ERROR_NO_CHEST)
    chestGravity(above);
}


//------------------------------------------------------------------------------
void Game::pickUpChest(int row, int col, int index)
{
  int worm = findWormAt(row, col);
  worms_[worm].addAmmo(chests_[index]->getWeapon());

  std::cout << worms_[worm].getName();
  std::cout << Sep::LEFT_BRACKET_STRING << worms_[worm].getId();
  std::cout << Sep::PICKED_UP_ONE_OF_MESSAGE;
  std::cout << gainedWeaponSwitch(chests_[index]->getWeapon()) << std::endl;

  delete chests_[index];
  chests_[index] = nullptr;
  chests_.erase(chests_.begin() + index);
}


//------------------------------------------------------------------------------
void Game::state()
{
  int player_on_move;
  if (worm_on_move_ > 3)
    player_on_move = 2;
  else
    player_on_move = 1;

  int current = findWormOnMove();

  std::cout << Sep::CURRENT_PLAYER_MESSAGE << player_on_move << Sep::NEW_LINE;
  std::cout << Sep::CUURENT_WORM_MESSAGE << worms_[current].getName() << 
        Sep::LEFT_BRACKET_STRING << worms_[current].getId() << ") \n\n";
  
  std::cout << Sep::WEAPONS_OF_CURRENT_WORM;
  
  std::string selected = "";// string to note if weapon is currently selected

  if (worms_[current].getWeapon() == Worm::GUN) //gun is done separately because
    selected = " *";                           // it has infinite ammo
  std::cout << Sep::GUN_AMMO_INF << selected << "\n";

  selected = "";

  int weapon_cntr;
  Worm::WeaponType weapon;
  int ammo; // here we store if the worm has ammunition of a certain type, and
            // how much

  for (weapon_cntr = 1; weapon_cntr <= NUMBER_OF_WEAPONS; weapon_cntr++)
  {
    weapon = static_cast<Worm::WeaponType>(weapon_cntr);
    ammo = worms_[current].getAmmo(weapon);
    if (ammo)
    {
      if (worms_[current].getWeapon() == weapon)
        selected = " *";
      std::cout << "  " << gainedWeaponSwitch(weapon) << Sep::AMMUNITION_MESSAGE
            << ammo <<  ")" << selected << std::endl;
    }
    selected = "";
  }
  
  // print information about the alive worms of the players
  std::cout << Sep::WORMS_OF_PLAYER_ONE;
  int counter = 0;
  while(worms_[counter].getId() <= 3)
  {
    std::cout << "  " << worms_[counter].getName() << " (";
    std::cout << worms_[counter].getId() << ") " << worms_[counter].getHealth();
    std::cout << Sep::HITPOINTS_MESSAGE << worms_[counter].getRow() << ", " <<
          worms_[counter].getColumn() << ") ~\n";
    counter++;
  }
  std::cout << Sep::WORMS_OF_PLAYER_TWO;
  while(counter < static_cast<int>(worms_.size()))
  {
    std::cout << "  " << worms_[counter].getName() << " (";
    std::cout << worms_[counter].getId() << ") " << worms_[counter].getHealth();
    std::cout << Sep::HITPOINTS_MESSAGE << worms_[counter].getRow() << ", " <<
              worms_[counter].getColumn() << ") *\n";
    counter++;
  }
}


//------------------------------------------------------------------------------
int Game::airstrike(int column)
{
  int current = findWormOnMove();

  // if user input has too big or negative numbers for column, ERROR
  if ((column > board_width_ - 1) || (column < 0))
  {
    std::cout << Sep::ERROR_INVALID_PARAMS;
    return Sep::RETURN_ERROR;
  }
  worms_[current].removeAmmo(Worm::AIRSTRIKE);
  
  bool missed = false;
  int row_counter = 0;
  
  while (1)
  {
    if (row_counter == board_height_)
    {
      missed = true;
      break; //we now know to print the missed message
    }
    if (map_[row_counter][column].getType() == Field::WATER)//if shot hit water
      return Sep::RETURN_OK;     // nothing happens, we return 0
    row_counter++;
    if (map_[row_counter][column].getType() != Field::AIR)
      break;
  }
  
  if (missed)
  {
    std::cout << SHOT_MISSED;
  }
  else if (map_[row_counter][column].getType() == Field::EARTH)
  {
    map_[row_counter][column].setType(Field::AIR);
    std::cout << Sep::SHOT_STRING << Sep::HIT_EARTH_AT_MESSAGE << row_counter 
          << ", " << column << ")\n";
  }
  else if (map_[row_counter][column].getType() == Field::WORM)
  {
    int worm = findWormAt(row_counter, column);
    worms_[worm].removeHealth(AIRSTRIKE_DAMAGE); 
    std::cout << Sep::SHOT_STRING << Sep::HIT_WORM_AT_MESSAGE << row_counter 
          << ", " << column << ")\n";
    if (worms_[worm].getHealth() <= 0) //if worm died due to damage-return
    {
      map_[row_counter][column].setType(Field::AIR);
      worms_[worm].death(KILLED_BY_WEAPON);
      worms_.erase(worms_.begin() + worm);
      return Sep::RETURN_OK;
    }
    // if worm survived, we print took damage message
    std::cout << worms_[worm].getName() << " (" << worms_[worm].getId() <<
          Sep::TOOK_MESSAGE << AIRSTRIKE_DAMAGE << Sep::HP_DAMAGE_MESSAGE;
  }
  else if (map_[row_counter][column].getType() == Field::CHEST)
  {
    int index = findChestAt(row_counter, column);
    delete chests_[index];
    chests_[index] = nullptr;
    chests_.erase(chests_.begin() + index);

    map_[row_counter][column].setType(Field::AIR);
    std::cout << Sep::SHOT_STRING << Sep::HIT_CHEST_AT_MESSAGE << row_counter 
          << ", " << column << ")\n";
  }
  return Sep::RETURN_OK;
}


//------------------------------------------------------------------------------
int Game::melee()
{
  int current = findWormOnMove();
  worms_[current].removeAmmo(Worm::MELEE);
  
  int row, col;
  row = worms_[current].getRow();
  col = worms_[current].getColumn();

  hitWithBat(row - 1, col);      // ABOVE US
  
  hitWithBat(row - 1, col + 1);  // ABOVE RIGHT
  
  hitWithBat(row, col + 1);      // RIGHT
  
  hitWithBat(row + 1, col + 1);  // DOWN RIGHT
  
  hitWithBat(row + 1, col);      // BELOW 
  
  hitWithBat(row + 1, col - 1);  // DOWN LEFT
  
  hitWithBat(row, col - 1);      // LEFT 
  
  hitWithBat(row - 1, col -1);   // ABOVE LEFT
    
  return Sep::RETURN_OK;
}


//------------------------------------------------------------------------------
void Game::hitWithBat(int new_row, int new_column)
{
  if ((new_row < 0) || (new_row > board_height_ - 1) || (new_column < 0) ||
        (new_column > board_width_ - 1)) // if hit outside map, nothing happens
    return;

  // Melee only affects worms, it can't destroy chests or earth fields
  if (map_[new_row][new_column].getType() == Field::WORM)
  {
    int worm = findWormAt(new_row, new_column);
    worms_[worm].removeHealth(MELEE_DAMAGE);

    std::cout << Sep::MELEE_HIT_WORM_MESSAGE << worms_[worm].getRow() <<
          ", " << worms_[worm].getColumn() << ")\n";

    if (worms_[worm].getHealth() <= 0)
    {
      map_[new_row][new_column].setType(Field::AIR);
      worms_[worm].death(KILLED_BY_WEAPON);
      worms_.erase(worms_.begin() + worm);

      int above = findWormAt(new_row, new_column);
      if (above != Sep::RETURN_ERROR_NO_WORM)
        gravity(above);
      return;
    }
    // if worm survived we print damage message
    std::cout << worms_[worm].getName() << " (" << worms_[worm].getId() <<
           Sep::TOOK_MESSAGE << Sep::MELEE_DAMAGE << Sep::HP_DAMAGE_MESSAGE;
  }
}


//------------------------------------------------------------------------------
int Game::teleport(int row, int col)
{
  int current = findWormOnMove();
  if ((row < 0) || (row > board_height_ - 1) || (col < 0) ||
        (col > board_width_ - 1)) // if user input is outside of map
  {
    std::cout << Sep::ERROR_INVALID_PARAMS;
    return Sep::RETURN_ERROR;
  }
  worms_[current].removeAmmo(Worm::TELEPORT);
  
  int current_row, current_col;
  current_row = worms_[current].getRow();
  current_col = worms_[current].getColumn();
  
  if (map_[row][col].getType() == Field::CHEST)
  {
    int chest_index = findChestAt(row, col);
    std::cout << worms_[current].getName() << Sep::LEFT_BRACKET_STRING << 
          worms_[current].getId();
    std::cout << Sep::PICKED_UP_ONE_OF_MESSAGE;
    std::cout << gainedWeaponSwitch(chests_[chest_index]->getWeapon()) <<
          Sep::NEW_LINE;
    worms_[current].addAmmo(chests_[chest_index]->getWeapon());
    delete chests_[chest_index];
    chests_[chest_index] = nullptr;
    chests_.erase(chests_.begin() + chest_index);// delete chest
  }
  else if (map_[row][col].getType() != Field::AIR) // we can only teleport into
  {                          // fields that are AIR or CHEST
    std::cout << Sep::ERROR_INVALID_TARGET;
    return Sep::RETURN_ERROR;
  }
  
  map_[row][col].setType(Field::WORM);
  map_[current_row][current_col].setType(Field::AIR);
  worms_[current].setCoordinates(row, col);
  gravity(current);
  
  int above = findWormAt(current_row - 1, current_col);
  if (above != Sep::RETURN_ERROR_NO_WORM)
    gravity(above);

  return Sep::RETURN_OK;
}


//------------------------------------------------------------------------------
std::string Game::gainedWeaponSwitch(Worm::WeaponType weapon)
{
  if (weapon == Worm::BAZOOKA)
    return Sep::BAZOOKA_STRING;
  if (weapon == Worm::BLOWTORCH)
    return Sep::BLOWTORCH_STRING;
  if (weapon == Worm::AIRSTRIKE)
    return Sep::AIRSTRIKE_STRING;
  if (weapon == Worm::MELEE)
    return Sep::MELEE_STRING;
  if (weapon == Worm::TELEPORT)
    return Sep::TELEPORT_STRING;
  return Sep::GUN_STRING;

}


//------------------------------------------------------------------------------
Worm::WeaponType Game::getWeaponOfWormOnMove()
{
  int worm = findWormOnMove();
  return worms_[worm].getWeapon();
}


//------------------------------------------------------------------------------
void Game::shotConsequence(int row, int col)
{
  int above = findChestAt(row - 1, col);
  if (above != Sep::RETURN_ERROR_NO_CHEST)
  {
    chestGravity(above);
    return; // if there was a chest, there couldn't be a worm, so we return
  }
  above = findWormAt(row - 1, col);
  if (above != Sep::RETURN_ERROR_NO_WORM)
    gravity(above);
  return;
}


//------------------------------------------------------------------------------
void Game::shotHit(int row, int col, int damage)
{

  std::string used_weapon;
  used_weapon = Sep::SHOT_STRING;
  if (damage == BLOWTORCH_DAMAGE)
    used_weapon = Sep::TORCH_STRING;
  if (map_[row][col].getType() == Field::CHEST) // if hit chest, delete it
  {
    map_[row][col].setType(Field::AIR);
    int index = findChestAt(row, col);
    delete chests_[index];
    chests_[index] = nullptr;
    chests_.erase(chests_.begin() + index);
    std::cout << used_weapon << Sep::HIT_CHEST_AT_MESSAGE << row << ", " 
          << col << ")\n";
  }
  else if (map_[row][col].getType() == Field::WORM) // if hit worm, damage it
  {
    int hit_worm = findWormAt(row, col);
    worms_[hit_worm].removeHealth(damage);
    std::cout << used_weapon << Sep::HIT_WORM_AT_MESSAGE << row << ", " 
          << col << ")\n";

    if (worms_[hit_worm].getHealth() <= 0)  // if worm died, we return
    {
      map_[row][col].setType(Field::AIR);
      worms_[hit_worm].death(KILLED_BY_WEAPON);
      worms_.erase(worms_.begin() + hit_worm);
      return;
    }
    // if worm survived we  output damage message
    std::cout << worms_[hit_worm].getName() << " (" << worms_[hit_worm].getId()
          << Sep::TOOK_MESSAGE << damage << Sep::HP_DAMAGE_MESSAGE;
  }
  else if (map_[row][col].getType() == Field::EARTH) // if hit earth, destry it
  {
    map_[row][col].setType(Field::AIR);
    std::cout << used_weapon << Sep::HIT_EARTH_AT_MESSAGE << row << ", " 
          << col << ")\n";
  }
}


//------------------------------------------------------------------------------
int Game::blowtorch(std::pair<int, int> coordinates_change)
{
  int worm = findWormOnMove();
  worms_[worm].removeAmmo(Worm::BLOWTORCH);//choose already checked for no ammo

  int row_change = coordinates_change.first;
  int column_change = coordinates_change.second;

  int row = worms_[worm].getRow();
  int col = worms_[worm].getColumn();

  int counter;
  bool hit = false; // used to know if we should print Shot missed essage
  for (counter = 0; counter < 5; counter++)
  {
    row += row_change;
    col += column_change;
    if (((col > board_width_ - 1) || (row < 0) || (row > board_height_ - 1)
        || (col < 0)) && (!hit)) // to be replaced with exception catch
    {
      std::cout << SHOT_MISSED;
      return Sep::RETURN_OK;
    }

    if (map_[row][col].getType() == Field::WATER) // if we hit water,
      return Sep::RETURN_OK;        // nothing should happen we just return

    if (map_[row][col].getType() == Field::AIR)
      continue;

    shotHit(row, col, BLOWTORCH_DAMAGE); // destroy or damage hit field
    hit = true;
  }

  row = worms_[worm].getRow(); //reset position to prep for shot consequence
  col = worms_[worm].getColumn();

  // go through the same fields again, applying gravity to objects above
  for (counter = 0; counter < 5; counter++)
  {
    row += row_change;
    col += column_change;
    shotConsequence(row, col);
  }
  return Sep::RETURN_OK;
}


//------------------------------------------------------------------------------
int Game::gun(std::pair<int, int> coordinates_change)
{
  int current_worm = findWormOnMove();
  int row = worms_[current_worm].getRow();
  int col = worms_[current_worm].getColumn();

  int row_change = coordinates_change.first;
  int column_change = coordinates_change.second;
  
  while (1)
  {
    row += row_change;
    col += column_change;
    if ((col > board_width_ - 1) || (row < 0) || (row > board_height_ - 1)
        || (col < 0)) // if outside of map
    {
      std::cout << SHOT_MISSED;
      return Sep::RETURN_OK;
    }
    if (map_[row][col].getType() == Field::WATER) // if we hit water, nothing
      return Sep::RETURN_OK;               // should happen so we return 0

    if (map_[row][col].getType() == Field::AIR)
      continue;

    break; // as soon as we hit a non-air field we break, and damage/destroy it
  }
  shotHit(row, col, GUN_DAMAGE);
  shotConsequence(row, col);
  return Sep::RETURN_OK;
}


//------------------------------------------------------------------------------
int Game::bazooka(std::pair<int, int> coordinates_change)
{
  int worm = findWormOnMove();
  worms_[worm].removeAmmo(Worm::BAZOOKA);
  
  int row = worms_[worm].getRow();
  int col = worms_[worm].getColumn();
  int row_change = coordinates_change.first;
  int column_change = coordinates_change.second;
  
  while (1)
  {
    row += row_change;
    col += column_change;
    if ((col > board_width_ - 1) || (row < 0) || (row > board_height_ - 1)
        || (col < 0)) // to be replaced with exception catch
    {
      std::cout << SHOT_MISSED;
      return Sep::RETURN_OK;
    }

    if (map_[row][col].getType() == Field::WATER)// if we hit water, nothing
      return Sep::RETURN_OK;        // nothing should happen we just return

    if (map_[row][col].getType() == Field::AIR)
      continue;

    break; // as soon as we hit a non-air field we break, and hit fields by AOE
  }
  
  shotHit(row, col, BAZOOKA_DAMAGE);     // MIDDLE FIELD-first hit
  shotHit(row - 1, col, BAZOOKA_DAMAGE); // ABOVE
  shotHit(row, col + 1, BAZOOKA_DAMAGE); // RIGHT Side
  shotHit(row + 1, col, BAZOOKA_DAMAGE); // BELOW
  shotHit(row, col - 1, BAZOOKA_DAMAGE); // LEFT Side

  shotConsequence(row, col);
  shotConsequence(row - 1, col);
  shotConsequence(row, col + 1);
  shotConsequence(row + 1, col);
  shotConsequence(row, col - 1);

  return Sep::RETURN_OK;
  
}


//------------------------------------------------------------------------------
void Game::increaseTurn()
{
  if (worm_of_player_one_ == worm_on_move_)
  {
    increasePlayerOne();
    
    if (findWormWithId(worm_of_player_two_) == Sep::RETURN_ERROR_NO_WORM)
      increasePlayerTwo();
    
    worm_on_move_ = worm_of_player_two_;
  }
  else  // worm_of_player_two_ == worm_on_move_
  {
    increasePlayerTwo();
    
    if (findWormWithId(worm_of_player_one_) == Sep::RETURN_ERROR_NO_WORM)
      increasePlayerOne();
    
    worm_on_move_ = worm_of_player_one_;
  }
  moved_ = false; // enables move for next worm's turn
}


//------------------------------------------------------------------------------
void Game::increasePlayerOne()
{
  if (findWormWithId(worm_of_player_one_) == Sep::RETURN_ERROR_NO_WORM)
  {
    if (worm_of_player_one_ == 1) //if it was worm with id 1, first worm in list
      worm_of_player_one_ = worms_[0].getId();// has to be next of P1
    else if (worm_of_player_one_ == 2) // if it was worm with id 2
    { //if worm with id 3 alive, it becomes  next worm of player one
      if (findWormWithId(3) != Sep::RETURN_ERROR_NO_WORM) 
        worm_of_player_one_ = 3;
      else
        worm_of_player_one_ = 1; //else, it has to be worm with id 1
    }
    else // worm_of_player_one_ == 3, first worm in vector is what we want
      worm_of_player_one_ = worms_[0].getId ();
    return;
  }// if next worm in vector is of P 2, go to start of vector
  if (worms_[findWormWithId(worm_of_player_one_) + 1].getId() > 3)
    worm_of_player_one_ = worms_[0].getId(); 
  else
  {
    worm_of_player_one_ = 
        worms_[findWormWithId(worm_of_player_one_) + 1].getId();
  }
}



//------------------------------------------------------------------------------
void Game::increasePlayerTwo()
{
  if (findWormWithId(worm_of_player_two_) == Sep::RETURN_ERROR_NO_WORM)
  {
    if (worm_of_player_two_ == 6)
    {
      int counter;
      for (counter = 0; counter <= static_cast<int>(worms_.size()); counter++)
      {
        if (worms_[counter].getId() > 3)
          break;
      }
      worm_of_player_two_ = worms_[counter].getId();
      return;
    }
    if (worm_of_player_two_ == 5) //if it was 5 and it died, we want final worm
      worm_of_player_two_ = worms_[worms_.size() - 1].getId();
    else // worm_of_p_two == 4
    {
      if (findWormWithId(5) != Sep::RETURN_ERROR_NO_WORM)
        worm_of_player_two_ = 5;
      else
        worm_of_player_two_ = 6;
    }
    return;
  }
  if (findWormWithId(worm_of_player_two_) ==
         static_cast<int>(worms_.size() - 1))
  {
    int counter;
    for (counter = 0; counter <= static_cast<int>(worms_.size()); counter++)
    {
      if (worms_[counter].getId() > 3)
        break;
    }
    worm_of_player_two_ = worms_[counter].getId();
  }
  else
  {
    worm_of_player_two_ = 
        worms_[findWormWithId(worm_of_player_two_) + 1].getId();
  }
}


//------------------------------------------------------------------------------
int Game::findWormWithId(int id)
{
  int counter = 0;
  for (counter = 0; counter <= static_cast<int>(worms_.size()); counter++)
  {
    if (worms_[counter].getId() == id)
      return counter;
  }
  return Sep::RETURN_ERROR_NO_WORM; // return -1 if no such worm alive
}

//------------------------------------------------------------------------------
int Game::findWormOnMove()
{
  int counter;
  for (counter = 0; counter < 6; counter++)
  {
    if (worms_[counter].getId() == worm_on_move_)
      return counter;
  }
  return Sep::RETURN_ERROR_NO_WORM; // return -1 if no worm is on move - ERR
}


//------------------------------------------------------------------------------
int Game::findWormAt(int row, int col)
{
  int worm_counter;
  for (worm_counter = 0; worm_counter < static_cast<int>(worms_.size()); 
          worm_counter++)
  {
    if ((worms_[worm_counter].getRow() == row) &&
           (worms_[worm_counter].getColumn() == col))
      return worm_counter;
  }
  return Sep::RETURN_ERROR_NO_WORM;// return -1 if no worm is on that postiion
}



//------------------------------------------------------------------------------
void Game::setFirstTurn()
{
  worm_on_move_ = worms_[0].getId();
  worm_of_player_one_ = worms_[0].getId();
  int counter;
  for (counter = 0; counter <= static_cast<int>(worms_.size()); counter++)
  {
    if (worms_[counter].getId() > 3)
      break;
  }
  worm_of_player_two_ = worms_[counter].getId();
  moved_ = false; // enable movve for first turn
}


//------------------------------------------------------------------------------
void Game::addWorms()
{
  int worm_counter;
  for (worm_counter = 0; worm_counter < 6; worm_counter++)
  {
    Worm new_worm;
    new_worm.setName(random_number_generator_.getWormName());
    new_worm.setCoordinates(0, random_number_generator_.getRandomInt(0, 
              board_width_ - 1));

    while (map_[0][new_worm.getColumn()].getType() == Field::WORM)
      new_worm.setCoordinates(0, random_number_generator_.getRandomInt(0, 
              board_width_ - 1));
    
    map_[0][new_worm.getColumn()].setType(Field::WORM);
    
    std::cout << Sep::SPAWNING_MESSAGE << new_worm.getName();
    
    new_worm.setId(worm_counter + 1);
    
    std::cout << " (" << new_worm.getId() << Sep::AT_MESSAGE;
    std::cout << new_worm.getRow() << ", " << new_worm.getColumn() << ')';
    std::cout << std::endl;
    new_worm.setRow(beginGravity(0, new_worm.getColumn()));
    
    if (new_worm.getRow() == FELL_INTO_WATER)
    {
      new_worm.death(FELL_INTO_WATER);
      continue;
    }
    worms_.push_back(new_worm);
  }
}

//------------------------------------------------------------------------------
int Game::beginGravity(int row, int col)
{
  // The fall
  while (1)
  {
    row += 1; // increment the row
    if (map_[row][col].getType() == Field::AIR) // if we find air
    {
      map_[row][col].setType(Field::WORM); // We set our worm there
      map_[row-1][col].setType(Field::AIR); // And set old position to AIR
      continue;
    }
    else // We hit something that isn't air
    {
      row -= 1; // reset the row to mark place of current Worm
      break;
    }
  }

  // And now we make sure that if we fell into water, worm disappears
  if (map_[row + 1][col].getType() == Field::WATER)
  {
    map_[row][col].setType(Field::AIR);
    return Sep::FELL_INTO_WATER;
  }
  
  // Return row to set new value in method where we called gravity(if neeeded)
  return row;
}

//------------------------------------------------------------------------------
void Game::gravity(int worm)
{
  int row = worms_[worm].getRow();
  int col = worms_[worm].getColumn();
  int above_row;

  if ((row - 1 < 0) || (map_[row - 1][col].getType() != Field::WORM))
    above_row = -1;
  else
    above_row = row - 1;

  int beginning_row = row;
  int damage_counter = -1;
  
  while (1)
  {
    row += 1;
    if (row == board_height_) // we fell out of the map
    {
      map_[beginning_row][col].setType(Field::AIR);
      worms_[worm].death(FELL_OUT_OF_MAP);
      worms_.erase(worms_.begin() + worm);
      int above = findWormAt(above_row, col);
      if (above != Sep::RETURN_ERROR_NO_WORM)
        gravity(above);
      return;
    }
    damage_counter += 1;
    if (map_[row][col].getType() == Field::CHEST)
    {
      //pick up chest
      int index = findChestAt(row, col);
      worms_[worm].setCoordinates(row, col);
      pickUpChest(row, col, index);
      map_[row][col].setType(Field::AIR);
      continue;
    }
    if(map_[row][col].getType() == Field::AIR)
    {
      continue;
    }
    else // We hit something that isn't air
    {
      damage_counter -= 1;
      break;
    }
  }
  
  map_[beginning_row][col].setType(Field::AIR);
  
  if (map_[row][col].getType() == Field::WATER)
  {
    worms_[worm].death(FELL_INTO_WATER);
    worms_.erase(worms_.begin() + worm);
    return;
  }
  else  // reset the row to mark place of current Worm???
  {
    row -= 1;
    map_[row][col].setType(Field::WORM);
    worms_[worm].setCoordinates(row, col);
  }
  
  if (damage_counter > 0)
  {
    damage_counter *= 10;
    worms_[worm].removeHealth(damage_counter);
    if (worms_[worm].getHealth() <= 0)
    {
      map_[row][col].setType(Field::AIR);
      worms_[worm].death(FELL_TO_DEATH);
      worms_.erase(worms_.begin() + worm);
    }
    else
    {
      std::cout << worms_[worm].getName() << " (" << worms_[worm].getId() <<
              Sep::TOOK_MESSAGE << damage_counter << Sep::FALL_DAMAGE_MESSAGE;
    }
  }
  
  int above = findWormAt(above_row, col);
  if (above != RETURN_ERROR_NO_WORM)
    gravity(above);
  
}


int Game::moveLogic(int steps)
{
  if (moved_)
  {
    std::cout << ERROR_NOT_ALLOWED;
    return Sep::ALREADY_MOVED;
  }
  moved_ = true;
  int direction;  // Moving left / right uses negative / positive integers
  if (steps > 0)
    direction = 1; // We set it to 1 if we should move right
  else
    direction = -1; // And we set it to -1 if we should move left
  return move(steps, direction);
}


//------------------------------------------------------------------------------
int Game::move(int steps, int direction)
{
  int worm = findWormOnMove();
  gravity(worm);  // We make sure that the worm falls if it can
  
  if (worms_[worm].getId() != worm_on_move_) // if some worm died in gravity
  {
    worm = findWormOnMove();// if our worm on move died in gravity we return -1
    if (worm == Sep::RETURN_ERROR_NO_WORM) 
    {
      return Sep::RETURN_ERROR;
    }
  }
  //if the worm doesn't need to move left or right, and only needs to fall
  if (!steps) // We can leave the method here, we did all our steps.
    return Sep::RETURN_OK;  
  
  int row = worms_[worm].getRow();
  int col = worms_[worm].getColumn();
  
  if ((col + direction < 0) || (col + direction > board_width_ - 1) ||
         (row - 1 < 0))
  {
    worms_[worm].death(FELL_OUT_OF_MAP);
    worms_.erase(worms_.begin() + worm);
    map_[row][col].setType(Field::AIR);
    int above_us = findWormAt(row - 1, col);
    if (above_us != Sep::RETURN_ERROR_NO_WORM)
      gravity(above_us);
    return Sep::RETURN_ERROR;
  }
  // If next to us is air (comment to long to be in same line)
  else if (map_[row][col + direction].getType() == Field::AIR)
  {
    map_[row][col + direction].setType(Field::WORM);
    map_[row][col].setType(Field::AIR); // And set old position to AIR
    worms_[worm].setCoordinates(row, col + direction); //We change coordinates
  }
  else if (map_[row][col + direction].getType() == Field::CHEST)
  {
    map_[row][col + direction].setType(Field::WORM); // We move our worm
    map_[row][col].setType(Field::AIR);
    worms_[worm].setCoordinates(row, col + direction);
  
    int index = findChestAt(row, col + direction);
    pickUpChest(row, col + direction, index);
    
    int chest_above = findChestAt(row - 1, col + direction);
    if (chest_above != Sep::RETURN_ERROR_NO_CHEST)
      chestGravity(chest_above);
  }
  // Else, next to us is not air or chest, so we check if above that is air
  else if (map_[row - 1][col + direction].getType() == Field::AIR)
  {
    if (map_[row - 1][col].getType() == Field::EARTH) // check for earth above
    {
      std::cout << Sep::WARNING_CANNOT_MOVE;
      return Sep::RETURN_OK; // We cannot climb, so we give a warning, return.
    }
    // Else, we are allowed to climb:
    map_[row - 1][col + direction].setType(Field::WORM); // We move our worm
    map_[row][col].setType(Field::AIR); // And set old position to AIR
    worms_[worm].setCoordinates(row - 1, col + direction);
  }
  else if (map_[row - 1][col + direction].getType() == Field::CHEST)
  {
    if (map_[row - 1][col].getType() == Field::EARTH)
    {
      std::cout << Sep::WARNING_CANNOT_MOVE;
      return Sep::RETURN_OK; // We cannot climb, so we give a warning, return.
    }
    map_[row - 1][col + direction].setType(Field::WORM); // We move our worm
    map_[row][col].setType(Field::AIR);// And set old position to AIR
    worms_[worm].setCoordinates(row - 1, col + direction);
    
    int index = findChestAt(row - 1, col + direction);
    pickUpChest(row -1, col + direction, index);
    
    int chest_above = findChestAt(row - 2, col + direction);
    if (chest_above != Sep::RETURN_ERROR_NO_CHEST)
      chestGravity(chest_above);
  }
  else// Else, we are not allowed to climb here so we give a warning
  {
    std::cout << Sep::WARNING_CANNOT_MOVE;
    return Sep::RETURN_OK;
  }
  steps -= direction;//decrease number of steps

  int above_us = findWormAt(row - 1, col);
  if (above_us != Sep::RETURN_ERROR_NO_WORM)
    gravity(above_us);
 
  if (move(steps, direction))  // Now we move again, but doing 1 step less
    return Sep::RETURN_ERROR; // If worm on move died, we return -1

  return Sep::RETURN_OK;
}




//------------------------------------------------------------------------------
void  Game::printOnMove()
{
  int current = findWormOnMove(); // current is now index of worm on move
  int player;
  if (worm_on_move_ > 3)
    player = 2;
  else
    player = 1;
  
  std::cout << Sep::PLAYER_MESSAGE << player << Sep::WORM_MESSAGE << 
        worms_[current].getName() << " (" << worm_on_move_ << Sep::AT_MESSAGE;
  std::cout <<worms_[current].getRow() << ", " << worms_[current].getColumn() 
        << Sep::READY_MESSAGE;
}


//------------------------------------------------------------------------------
void Game::printMap()
{
  //Create 2 int variables to count our columns and rows, and 1 char for worms
  int column_counter;
  int row_counter;
  char to_be_printed;
  std::cout << Sep::CURRENT_MAP_MESSAGE;
  std::cout << "  ";
  for (column_counter = 0; column_counter < board_width_; column_counter++)
  {
    std::cout << column_counter % 10;//This will make our column header
  }
  std::cout << "\n";
  
  // Now we want to print out our map
  for (row_counter = 0; row_counter < board_height_; row_counter++)
  {
    std::cout << std::setw(2) << row_counter;// First, we print current row
    // We go column by column in the current row, an print out the Field
    for (column_counter = 0; column_counter < board_width_; column_counter++)
    {
      to_be_printed = map_[row_counter][column_counter].getCharacter();
      if (to_be_printed)
        std::cout << to_be_printed;
      else //now wo differentiate between worms of P1 and P2
      {
        int current_worm = findWormAt(row_counter, column_counter);
        if (worms_[current_worm].getId() > 3)
          std::cout << '*';
        else
          std::cout << '~';
      }
    }
    // At the end of the row, we again print out the row we are in, then '\n'
    std::cout << std::setw(2) << row_counter << "\n";
  }
  
  //We now print the column footer, using modulo to just get the last digit
  std::cout << "  ";
  for (column_counter = 0; column_counter < board_width_; column_counter++)
  {
    std::cout << column_counter % 10;
  }
  std::cout << "\n";
}

//------------------------------------------------------------------------------
int Game::read_input_until_char(std::ifstream* input, std::string &read_line,
            char wanted_char)
{
  // We keep reading new lines until we meet either our wanted char
  while (std::getline(*input, read_line))
  {
    //We erase the whitespaces at the end of the line
    read_line.erase(read_line.find_last_not_of(" \t\r") + 1);

    // If our line is empty(only whitespaces), we skip this line and continue
    if (read_line.find_first_not_of(" \t\r") == std::string::npos)
      continue;

    // If our line begis with a '#', it's a comment, so we skip and continue
    if (read_line[0] == '#')
      continue;

    // Once we meet the charachter we wanted, we return 0 
    if (read_line[0] == wanted_char)
      return Sep::RETURN_OK;
    
    // Else, we found a charachter that we don't want, and also isn't a '#'
    // The file is then invalid, so we output our Error message, and return -1
    std::cout << Sep::ERROR_INVALID_FILE;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  // If we reached the end of the file, the file is invalid so 
  std::cout << Sep::ERROR_INVALID_FILE; // we output our Error message
  return Sep::RETURN_ERROR_INVALID_CFG_FILE; // And we return -1
}


//------------------------------------------------------------------------------
int Game::loadConfig(std::string cfg_file)
{
  
  std::ifstream* input = nullptr;
  try
  {
    input = new std::ifstream(cfg_file);
  }
  catch (std::bad_alloc)
  {
    std::cout << ERROR_BAD_MEMORY;
    delete input;
    return RETURN_ERROR_MEMORY;
  }
  std::string read_line; // and we create a string to load lines into

  if (!input->is_open()) //check if input-file stream is opened successfully
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }

  std::getline(*input, read_line);

  // We check if our file begins with the magic value "#SWORMS"
  if (read_line != "#SWORMS")
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  // We now go down in the file until we meet an 'S' (for SIZE:)
  if (read_input_until_char(input, read_line, 'S'))
  {
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  
  std::stringstream size(read_line);
  std::vector<std::string> words(2, "");
  size >> words[0] >> board_width_ >> board_height_;
  if (size.fail())
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  size >> words[1];  
  if ((words[0] != "SIZE:") || (words[1].size())) 
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
   //if numbers too big or too small, ERROR INVALID FILE
  if (((board_height_ > MAX_BOARD_SIZE) || (board_height_ < MIN_BOARD_SIZE)) ||
        ((board_width_ > MAX_BOARD_SIZE) || (board_width_ < MIN_BOARD_SIZE)))
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  map_.resize(board_height_, std::vector<Field>());// resize to board_height_
  
  int row_counter;
  int column_counter;
  // We now go down in the file until we meet an 'M' (for MAP:)
  if (read_input_until_char(input, read_line, 'M'))
  {
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  if (read_line != "MAP:") // our line should just contain "MAP:"
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  if (read_input_until_char(input, read_line, 'A')) // go down till 'A'
  {
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }

  for (row_counter = 0; row_counter < board_height_ - 1; row_counter++)
  {
    int result_of_read = readLineOfMap(read_line, input, row_counter);
    if (result_of_read == Sep::RETURN_ERROR_MEMORY)
    {
      delete input;
      return Sep::RETURN_ERROR_MEMORY;
    }
    if (result_of_read == Sep::RETURN_ERROR_INVALID_CFG_FILE)
    {
      std::cout << Sep::ERROR_INVALID_FILE;
      delete input;
      return Sep::RETURN_ERROR_INVALID_CFG_FILE;
    }
  }
  // Must now check if first line has non-Air elements(these are prohibited)
  for (column_counter = 0; column_counter < board_width_; column_counter++)
  {
    if (map_[0][column_counter].getType() != Field::AIR)
    {
      std::cout << Sep::ERROR_INVALID_FILE;
      delete input;
      return Sep::RETURN_ERROR_INVALID_CFG_FILE;
    }
  }  

  // Final line must be done separately from other lines
  if (static_cast<int>(read_line.length()) != board_width_)
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    delete input;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }

  //Now for each char in our string we check validity
  for  (column_counter = 0; column_counter < board_width_; column_counter++)
  {
    Field temp_field(Field::AIR);
    map_[board_height_ - 1].push_back(temp_field);
    if (read_line[column_counter] == 'E')
      map_[board_height_ - 1][column_counter].setType(Field::EARTH);

    else if (read_line[column_counter]  == 'W')
      map_[board_height_ - 1][column_counter].setType(Field::WATER);

    else
    {
      std::cout << Sep::ERROR_INVALID_FILE;
      delete input;
      return Sep::RETURN_ERROR_INVALID_CFG_FILE;
    }
  }
  
  while (!input->eof())// We also ckeck that there is no input after the map
  {
    std::getline(*input, read_line);// Read the next line
    read_line.erase(read_line.find_last_not_of(" \t\r") + 1);
    if (read_line.size())//If it has any non-whitespace chars, file is invalid
    {
      std::cout << Sep::ERROR_INVALID_FILE;
      delete input;
      return Sep::RETURN_ERROR_INVALID_CFG_FILE;
    }
  }

  input->close();// Close our input file stream and return 0, everything was OK
  delete input;
  return Sep::RETURN_OK;
}


int Game::readLineOfMap(std::string &line, std::ifstream* input, int row)
{
  int column_counter;
  if (static_cast<int>(line.length()) != board_width_)
  {
    std::cout << Sep::ERROR_INVALID_FILE;
    return Sep::RETURN_ERROR_INVALID_CFG_FILE;
  }
  // We check each character in our line for validity, and set types
  for (column_counter = 0; column_counter < board_width_; column_counter++)
  {
    Field temp_field(Field::AIR);
    try
    {
      map_[row].push_back(temp_field);
    }
    catch(std::bad_alloc)
    {
      std::cout << Sep::ERROR_BAD_MEMORY;
      return Sep::RETURN_ERROR_MEMORY;
    }
    if (line[column_counter] == 'E')
      map_[row][column_counter].setType(Field::EARTH);
    else if (line[column_counter] == 'A')
      map_[row][column_counter].setType(Field::AIR);
    else
    {
      std::cout << Sep::ERROR_INVALID_FILE;
      return Sep::RETURN_ERROR_INVALID_CFG_FILE;
    }
  }
  std::getline(*input, line); // Now we load our next line
  line.erase(line.find_last_not_of(" \t\r") + 1);
  return Sep::RETURN_OK;
}