#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <iostream>
#include "Config.hpp"

//------------------------------------------------------------------------------
//Typical exception
//------------------------------------------------------------------------------

class Exception {

public:
  Exception(std::string className, std::string functionName, std::string message);
  ~Exception();

  std::string what();

private:
  std::string className, functionName, message;

};


#endif
