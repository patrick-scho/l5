#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

namespace Lex
{
  struct Location
  {
    unsigned int index = 0;
    unsigned int row = 0;
    unsigned int col = 0;
  };

  enum class TokenType
  {
    Word, Integer, Operator,
    ParenL, ParenR, BracketL, BracketR, BraceL, BraceR,
    Comma, Semicolon
  };
  struct Token
  {
    TokenType type;
    std::string str;
    Location loc;
  };

  bool isWhitespace(int c)
  {
    return
      c == ' ' ||
      c == '\t' ||
      c == '\r' ||
      c == '\n';
  }

  bool isNumber(int c)
  {
    return c >= '0' && c <= '9';
  }

  bool isFirstWord(int c)
  {
    return
      (c >= 'a' && c <= 'z') ||
      (c >= 'A' && c <= 'Z') ||
       c == '_';
  }

  bool isWord(int c)
  {
    return isFirstWord(c) || isNumber(c);
  }

  bool isPunctuation(int c)
  {
    return
      c == '(' ||
      c == ')' ||
      c == '[' ||
      c == ']' ||
      c == '{' ||
      c == '}' ||
      c == ',' ||
      c == ';';
  }

  Token getPunctuationToken(int c)
  {
    Token result;
    switch (c)
    {
    case '(': result.type = TokenType::ParenL;    break;
    case ')': result.type = TokenType::ParenR;    break;
    case '[': result.type = TokenType::BracketL;  break;
    case ']': result.type = TokenType::BracketR;  break;
    case '{': result.type = TokenType::BraceL;    break;
    case '}': result.type = TokenType::BraceR;    break;
    case ',': result.type = TokenType::Comma;     break;
    case ';': result.type = TokenType::Semicolon; break;
    }
    return result;
  }

  

  std::vector<Token> lex(std::istream & in)
  {
    enum class LexState {
      Neutral, Word, Number, Operator
    };

    Location currentLoc;
    std::vector<Token> result;
    LexState state = LexState::Neutral;

    char c;
    while (in.get(c))
    {
      if (state == LexState::Number)
      {
        if (isNumber(c))
          result.back().str += c;
        else
          state = LexState::Neutral;
      }
      else if (state == LexState::Word)
      {
        if (isWord(c))
          result.back().str += c;
        else
          state = LexState::Neutral;
      }
      else if (state == LexState::Operator)
      {
        if (! isWord(c) && ! isWhitespace(c) && ! isPunctuation(c))
          result.back().str += c;
        else
          state = LexState::Neutral;
      }
      
      if (state == LexState::Neutral)
      {
        if (isPunctuation(c))
        {
          state = LexState::Neutral;
          Token newToken = getPunctuationToken(c);
          newToken.loc = currentLoc;
          result.push_back(newToken);
        }
        else if (isFirstWord(c))
        {
          state = LexState::Word;
          Token newToken;
          newToken.type = TokenType::Word;
          newToken.loc = currentLoc;
          newToken.str += c;
          result.push_back(newToken);
        }
        else if (isNumber(c))
        {
          state = LexState::Number;
          Token newToken;
          newToken.type = TokenType::Integer;
          newToken.loc = currentLoc;
          newToken.str += c;
          result.push_back(newToken);
        }
        else if (! isWhitespace(c))
        {
          state = LexState::Operator;
          Token newToken;
          newToken.type = TokenType::Operator;
          newToken.loc = currentLoc;
          newToken.str += c;
          result.push_back(newToken);
        }
      }

      if (isWhitespace(c))
        state = LexState::Neutral;

      currentLoc.index++;
      currentLoc.col++;
      if (c == '\n')
      {
        currentLoc.row++;
        currentLoc.col = 0;
      }
    }

    return result;
  }
}