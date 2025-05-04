//----------------------------------------------------------------------------------------------------------------------
//
// The CommandLine class contains functionality for converting user input into a Command object.
// (Source: Assignment 1)
//
// Author: Tutors
//---------------------------------------------------------------------------------------------------------------------
//

#include <iostream>
#include "Utils.hpp"
#include "Command.hpp"
#include "CommandLine.hpp"

#include <algorithm>

void CommandLine::removeTrailingWhitespaces(std::string& string)
{
  size_t end = string.find_last_not_of(' ');
  string = end == std::string::npos ? "" : string.substr(0, end + 1);
}

void CommandLine::removeLeadingWhitespace(std::string& string)
{
  auto start = string.find_first_not_of(' ');
  string = start == std::string::npos ? "" : string.substr(start);
}

void CommandLine::removeWhitespacesAtEnds(std::string& string)
{
  removeLeadingWhitespace(string);
  removeTrailingWhitespaces(string);
}

void CommandLine::stringToVector(const std::string& string, std::vector<std::string>& vector, char delimiter)
{
  size_t position = 0;
  size_t position2;
  std::string split_word;
  while ((position2 = string.find(delimiter, position)) != std::string::npos)
  {
    split_word = string.substr(position, position2 - position);
    position = position2 + 1;
    removeWhitespacesAtEnds(split_word);
    if (!split_word.empty())
    {
      vector.push_back(split_word);
    }
  }
  vector.push_back(string.substr(position));
}

//Heavily changed from the original CommandLine readCommand, however all the param and invalid checks are made in the Command Class, and should work.
Command CommandLine::readCommand()
{
  //print "<CURRENT_PLAYER>> "
  std::string input;
  std::getline(std::cin, input);
  removeWhitespacesAtEnds(input);
  Utils::toLower(input);
  std::vector<std::string> vectorised_string;
  stringToVector(input, vectorised_string, ' ');
  Command command(vectorised_string);
  switch (command.getType())
  if (command.getType() == CommandType::INVALID)
  {
    //TODO: Throw error to catch in main
    
    //print message of the key "E_UNKNOWN_COMMAND"
  }
  if (command.getType() == CommandType::WRONG_PARAM)
  {
    //TODO: Throw error to catch in main
    
    //print message of the key "E_INVALID_PARAM_COUNT"
  }
  return command;
}
