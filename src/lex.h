#pragma once

#include <string>
#include <vector>

namespace Lex
{
  struct Location
  {
    unsigned int index, row, col;
  };

  enum class TokenType
  {
    Word, Integer, Operator
  };
  struct Token
  {
    TokenType type;
    std::string str;
    Location loc;
  };

  std::vector<Token> lex(std::istream & in)
  {
    Location currentLoc;
    std::vector<Token> result;

    return result;
  }
}