//------------------------------------------------------------------------------
// Field.cpp
// 
//------------------------------------------------------------------------------
//

#include "Field.h"

using Sep::Field;

//------------------------------------------------------------------------------
Field::Field(FieldType type) : type_(type)
{
}

//------------------------------------------------------------------------------
char Field::getCharacter()
{
  if (type_ == Field::AIR) // AIR should be represented via a space
    return ' ';
  if (type_ == Field::EARTH)  // Earth should be represented via an 'E'
    return 'E';
  if (type_ == Field::WATER)  // Water should be represented via a 'W'
    return 'W';
  if (type_ == Field::CHEST)
    return '#';
  else  // The only remaining option is a worm, so we return nothing,
    return '\0';  // we will determine if it's a star or tilda in caller method
}

//------------------------------------------------------------------------------
Field::FieldType Field::getType()
{
  return type_;
}

//------------------------------------------------------------------------------
void Field::setType(FieldType type)
{
  type_ = type;
}
