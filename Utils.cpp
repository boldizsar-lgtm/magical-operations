//---------------------------------------------------------------------------------------------------------------------
//
// The Utils class contains some useful functions for use in other classes. (Source: Assignment 1)
//
// Author: Tutors, Group 026
//---------------------------------------------------------------------------------------------------------------------
//

#include "Utils.hpp"

#include <algorithm>
#include <string>
#include <sstream>

bool Utils::decimalStringToInt(const std::string& str, int& number)
{
  size_t position = 0;

  try
  {
    number = std::stoul(str, &position, 10);
  }
  catch (std::exception&)
  {
    return false;
  }

  return position == str.length();
}

bool Utils::decimalStringToFloat(const std::string& str, float& floating)
{
  size_t position = 0;

  try
  {
    floating = std::stof(str, &position);
  }
  catch (std::exception&)
  {
    return false;
  }

  return position == str.length();
}

void Utils::toLower(std::string& string)
{
  for(char& character : string)
  {
    character = tolower(static_cast<unsigned char>(character));
  }
}
