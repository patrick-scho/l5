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

  void advanceLoc(Location & loc, char c)
  {
    loc.index++;
    loc.col++;
    if (c == '\n')
    {
      loc.row++;
      loc.col = 0;
    }
  }

  const char punctuationChars[] = "()[]{},;";
  const char operatorChars[] = ".+-*/<>|^°!%&?=~:";
  enum class TokenType
  {
    ParenL , ParenR, BracketL, BracketR, BraceL, BraceR,
    Comma, Semicolon,
    Word, Integer, Operator, String,
    None,
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
    for (char p : punctuationChars)
      if (c == p)
        return true;
    return false;
  }

  Token getPunctuationToken(int c)
  {
    Token result;
    for (int i = 0; i < sizeof(punctuationChars); i++)
      if (c == punctuationChars[i])
        result.type = (TokenType)i;
    return result;
  }

  bool isOperator(int c)
  {
    for (char o : operatorChars)
      if (c == o)
        return true;
    return false;
  }

  

  std::vector<Token> lex(std::istream & in)
  {
    Location currentLoc;
    std::vector<Token> result;

    char c;
    in.get(c);

    while (in.good())
    {
      if (isPunctuation(c))
      {
        Token newToken = getPunctuationToken(c);
        newToken.loc = currentLoc;
        result.push_back(newToken);
        
        if (! in.get(c))
          break;
        advanceLoc(currentLoc, c);
      }
      else if (isFirstWord(c))
      {
        Token newToken;
        newToken.type = TokenType::Word;
        newToken.loc = currentLoc;
        newToken.str += c;
        while (in.get(c))
        {
          advanceLoc(currentLoc, c);
          if (isWord(c))
            newToken.str += c;
          else
            break;
        }
        result.push_back(newToken);
      }
      else if (isNumber(c))
      {
        Token newToken;
        newToken.type = TokenType::Integer;
        newToken.loc = currentLoc;
        newToken.str += c;
        while (in.get(c))
        {
          advanceLoc(currentLoc, c);
          if (isNumber(c))
            newToken.str += c;
          else
            break;
        }
        result.push_back(newToken);
      }
      else if ('"' == c)
      {
        Token newToken;
        newToken.type = TokenType::String;
        newToken.loc = currentLoc;
        newToken.str += c;
        while (in.get(c))
        {
          advanceLoc(currentLoc, c);
          newToken.str += c;
          if ('"' == c)
            break;
        }
        if (! in.get(c))
          break;
        advanceLoc(currentLoc, c);
        result.push_back(newToken);
      }
      else if ('/' == c && '/' == in.peek())
      {
        in.get(c);
        advanceLoc(currentLoc, c);
        while (in.get(c))
        {
          advanceLoc(currentLoc, c);
          if (c == '\n')
            break;
        }
      }
      else if ('/' == c && '*' == in.peek())
      {
        int level = 1;

        in.get(c);
        advanceLoc(currentLoc, c);
        while (in.get(c))
        {
          advanceLoc(currentLoc, c);
          if ('*' == c && '/' == in.peek() ||
              '/' == c && '*' == in.peek())
          {
            if ('/' == c)
              level++;
            else
              level--;
              
            in.get(c);
            advanceLoc(currentLoc, c);
            in.get(c);
            advanceLoc(currentLoc, c);

            if (level == 0)
              break;
          }
        }
      }
      else if (isWhitespace(c))
      {
        advanceLoc(currentLoc, c);
        if (! in.get(c))
          break;
      }
      else if (isOperator(c))
      {
        Token newToken;
        newToken.type = TokenType::Operator;
        newToken.loc = currentLoc;
        newToken.str += c;
        
        while (in.get(c))
        {
          if ('/' == c && '/' == in.peek())
            break;
          if ('/' == c && '*' == in.peek())
            break;
          advanceLoc(currentLoc, c);
          if (isOperator(c))
            newToken.str += c;
          else
            break;
        }
        result.push_back(newToken);
      }
    }

    return result;
  }
}