#pragma once

#include <string>
#include <iostream>

namespace Log
{
  template<typename ... Ts>
  void error(const std::string & msg, Ts ...ts)
  {
    char buffer[1024];
    snprintf(buffer, 1024, msg.c_str(), ts...);
    std::cerr << "[ERROR] " << buffer << std::endl;
  }
  template<typename ... Ts>
  void warning(const std::string & msg, Ts ...ts)
  {
    char buffer[1024];
    snprintf(buffer, 1024, msg.c_str(), ts...);
    std::cerr << "[WARNING] " << buffer << std::endl;
  }
  template<typename ... Ts>
  void info(const std::string & msg, Ts ...ts)
  {
    char buffer[1024];
    snprintf(buffer, 1024, msg.c_str(), ts...);
    std::cerr << "[INFO] " << buffer << std::endl;
  }
}