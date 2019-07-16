//------------------------------------------------------------------------------
// Chest.h
//
// Declarations for Chest class.
// 
//------------------------------------------------------------------------------
//

#ifndef CHEST_H
#define CHEST_H

#include "Worm.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  // Chest Class
  // class to store individual chests, their location and the weapon that they
  // hold
  //
  class Chest
  {
    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Chest(int col, int weapon);
  
  
      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getRow();
      int getCol();
      Worm::WeaponType getWeapon();
  
      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setRow(int row);
      void setCol(int col);
      void setCoordinates(int row, int col);

      //-----------------------------------------------------------------------
      // Destructor
      // 
      ~Chest();
      
      
    private:
  
      //------------------------------------------------------------------------
      // The row that the chest is in
      //
      int row_;
  
      //------------------------------------------------------------------------
      // The column that the chest is in
      //
      int col_;
  
      //------------------------------------------------------------------------
      // The weapon that the chest holds, 0-4 (bazooka to airstirke)
      //
      int weapon_;
  };
}


#endif  // CHEST_H
