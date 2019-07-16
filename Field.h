//------------------------------------------------------------------------------
// Field.h
//
// Declarations for Field class.
// 
//------------------------------------------------------------------------------
//

#ifndef FIELD_H
#define FIELD_H

namespace Sep
{
  //----------------------------------------------------------------------------
  // Field Class
  // class to store individual fields on the map
  //
  class Field
  {

    public:
      
      //------------------------------------------------------------------------
      // Enum for possible types of field
      //
      enum FieldType {AIR, EARTH, WATER, WORM, CHEST};

      //------------------------------------------------------------------------
      // Constructor
      //
      Field(FieldType type);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      char getCharacter();

      FieldType getType();

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setType(FieldType type);

    private:

      //------------------------------------------------------------------------
      // Variable for storing the type of the Field instance
      // contains one of the 4 members od the (public) enum FieldType
      //
      Field::FieldType type_;

  };
  
}

#endif //FIELD_H
