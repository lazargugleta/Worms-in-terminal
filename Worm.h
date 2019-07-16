//------------------------------------------------------------------------------
// Worm.h
//------------------------------------------------------------------------------
//

#ifndef WORM_H
#define WORM_H

#include <string>
#include <iostream>
#include <map>


namespace Sep
{
  //----------------------------------------------------------------------------
  // Worm Class
  // class to store individual worm objects, their name, ID, health, ammo
  // and selected weapon
  //
  class Worm
  {
    public:
  
      //------------------------------------------------------------------------
      // Constructor
      //
      Worm();
  
      //------------------------------------------------------------------------
      // enum for possible types of weapons
      //
      enum WeaponType {GUN, BAZOOKA, TELEPORT, BLOWTORCH, MELEE, AIRSTRIKE};
  
  
      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setColumn(int col);
      void setRow(int row);
      void setCoordinates(int row, int col);
      void setHealth(int health);
      void setId(int id);
      void setName(std::string name);
      void selectWeapon(WeaponType type);
  
      //------------------------------------------------------------------------
      // removeHealth Method
      // Used to decrease the health of the worm by the passed amount
      // @param int damage - the amount of health we want to remove
      //
      void removeHealth(int damage);
  
      //------------------------------------------------------------------------
      // removeAmmo Method
      // Used to decrease the ammo of passed type by 1
      // @param WeaponType type - the sort of weapon we want to decrease the
      // ammo of
      //
      void removeAmmo(WeaponType type);
  
  
      //------------------------------------------------------------------------
      // addAmmo Method
      // Used to increase the ammo of passed type by 1
      // @param WeaponType type - the sort of weapon we want to increase the
      // ammo of
      //
      void addAmmo(WeaponType type);
      
      
      
      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getColumn();
      int getRow();
      int getHealth();
      std::string getName();
      int getId();
      int getAmmo(WeaponType type);
      WeaponType getWeapon();
  
      //------------------------------------------------------------------------
      // Death Method
      // Used to announce / print that the worm died
      // @param int cause the cause of the death of the worm- fell to death/
      // into water / out of map/ killed by weapon
      //
      void death(int cause);
  
  
    private:
      //------------------------------------------------------------------------
      // column that the worm is in
      //
      int col_;
  
      //------------------------------------------------------------------------
      // row that the worm is in
      //
      int row_;
  
      //------------------------------------------------------------------------
      // health the health that the worm has remaining
      //
      int health_;
  
      //------------------------------------------------------------------------
      // name of the worm
      //
      std::string name_;
      
      //------------------------------------------------------------------------
      // ID of the worm
      //
      int worm_id_;
  
      //------------------------------------------------------------------------
      // weapon that is currently selected
      //
      Worm::WeaponType selected_weapon_;
  
      //------------------------------------------------------------------------
      // map that stores the ammo for each weapon
      //
      std::map<Worm::WeaponType, int> ammo_;
  
  };
}


#endif //WORM_H
