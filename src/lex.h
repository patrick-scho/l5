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

  const char punctuation[] = {
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    ',',
    ';',
    ':',
  };
  enum class TokenType
  {
    ParenL , ParenR, BracketL, BracketR, BraceL, BraceR,
    Comma, Semicolon, Colon,
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
    for (char p : punctuation)
      if (c == p)
        return true;
    return false;
  }

  Token getPunctuationToken(int c)
  {
    Token result;
    for (int i = 0; i < sizeof(punctuation); i++)
      if (c == punctuation[i])
        result.type = (TokenType)i;
    return result;
  }

  

  std::vector<Token> lex(std::istream & in)
  {
    enum class LexState {
      Neutral, Word, Number, Operator, String,
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
      else if (state == LexState::String)
      {
        if ('"' != c)
        {
          result.back().str += c;
        }
        else
        {
          if (in.get(c))
            state = LexState::Neutral;
          else
            break;
        }
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
        else if ('"' == c)
        {
          state = LexState::String;
          Token newToken;
          newToken.type = TokenType::String;
          newToken.loc = currentLoc;
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