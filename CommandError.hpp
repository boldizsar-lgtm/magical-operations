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
    CommandError(const CommandError& error) = delete;
    ~CommandError() = default;
    virtual const char* what() const noexcept override
    {
     return message_.c_str();
    }
};

#endif //ERROR_HPP
