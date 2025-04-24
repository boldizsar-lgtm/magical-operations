//---------------------------------------------------------------------------------------------------------------------
//
// The Utils class contains some useful functions for use in other classes (Source: Assignment 1).
//
// Author: Tutors, Group 026
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

class Utils
{
public:
  //------------------------------------------------------------------------------------------------------------------
  // Constructor, Copy-Constructor and Destructor are deleted, as this class should never be instantiated.
  //
  Utils() = delete;
  Utils(const Utils&) = delete;
  ~Utils() = delete;

  //------------------------------------------------------------------------------------------------------------------
  // This function converts a string into an integer. The conversion is only successful, if all
  // elements of the string are converted.
  //
  // @param str - string that should be converted
  // @param int_number - the converted number
  // @return true, if conversion was successful, false otherwise
  //
  static bool decimalStringToInt(const std::string& str, int& int_number);

  //------------------------------------------------------------------------------------------------------------------
  // This function converts a string into a float. The conversion is only successful, if all
  // elements of the string are converted.
  //
  // @param str - string that should be converted
  // @param float_number - the converted number
  // @return true, if conversion was successful, false otherwise
  //
  static bool decimalStringToFloat(const std::string& string, float& float_number);

  //------------------------------------------------------------------------------------------------------------------
  // This function converts a string to a string with lowercase letters.
  //
  // @param string& - reference to the string that should be converted
  // @return no return value
  //
  static void toLower(std::string& string);
};

#endif // UTILS_HPP

