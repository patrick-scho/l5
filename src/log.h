#pragma once

#include <string>
#include <iostream>

namespace Log
{
  void error(const std::string & msg)
  {
    std::cout << msg << std::endl;
  }
  void warning(const std::string & msg)
  {
    std::cout << msg << std::endl;
  }
  void info(const std::string & msg)
  {
    std::cout << msg << std::endl;
  }
}