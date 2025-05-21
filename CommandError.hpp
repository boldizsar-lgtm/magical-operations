//---------------------------------------------------------------------------------------------------------------------
// CommandError.hpp
//
// The CommandError class is our own exeption class, that saves the type of the command error and determines the error
// message accordingly.
//
// Group: 026
//
// Author: Boldizsar Kiss, 12417350,
//         Lenon Osmani, 12303674,
//         Katharina Hartmann, 12219843
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include <string>
#include "Config.hpp"

class CommandError : public std::exception
{
  private:
    std::string message_;
  public:
    CommandError(std::string key)
    {
      //getting the correct error message out of the messages map
      message_ = Config::getInstance().getMessage(key);
    };
    CommandError(const CommandError& error) = default;
    ~CommandError() = default;
    virtual const char* what() const noexcept override
    {
     return message_.c_str();
    }
};

#endif //ERROR_HPP
