#pragma once

#include "lex.h"
#include "log.h"

#include <vector>
#include <memory>
#include <variant>
#include <optional>


namespace Parse
{
  struct Word { std::string val; };
  struct Operator { std::string val; };
  struct String { std::string val; };
  struct Integer { long val; };
  
  using Value = std::variant<
    Word,
    Operator,
    String,
    Integer
  >;

  struct Node
  {
    Value primary;

    std::optional<Value> secondary;

    std::optional<std::vector<Node>> parens;
    std::optional<std::vector<Node>> braces;
  };

  Lex::Token getNextToken(const std::vector<Lex::Token> & tokens, int & index)
  {
    if (index >= tokens.size())
      return Lex::Token { Lex::TokenType::None };
    return tokens[index];
  }

  Value parseValue(Lex::Token t)
  {
    if (t.type == Lex::TokenType::Word)
      return Word { t.str };
    else if (t.type == Lex::TokenType::Operator)
      return Operator { t.str };
    else if (t.type == Lex::TokenType::String)
      return String { t.str };
    else if (t.type == Lex::TokenType::Integer)
      return Integer { std::atol(t.str.c_str()) };
    else
      Log::error("Expected Value at %d, %d", t.loc.row, t.loc.col);

    return Value();
  }

  Node parseNode(const std::vector<Lex::Token> & tokens, int & index, bool parseOp = true)
  {
    Node result;

    auto t = tokens[index];

    if (t.type == Lex::TokenType::ParenL)
    {
      index++;
      result = parseNode(tokens, index);
      if (tokens[index].type != Lex::TokenType::ParenR)
        Log::error("Expected ) at %d, %d", tokens[index].loc.row, tokens[index].loc.col);
      index++;
      t = getNextToken(tokens, index);
    }
    else
    {
      result.primary = parseValue(t);

      index++;
      t = getNextToken(tokens, index);

      if (t.type == Lex::TokenType::Colon)
      {
        index++;
        t = getNextToken(tokens, index);

        result.secondary = parseValue(t);
        index++;
      }

      t = getNextToken(tokens, index);
      
      if (t.type == Lex::TokenType::ParenL)
      {
        index++;
        t = getNextToken(tokens, index);
    
        result.parens.emplace();
        while (tokens[index].type != Lex::TokenType::ParenR)
          result.parens->push_back(parseNode(tokens, index));
    
        index++;
      }

      t = getNextToken(tokens, index);
    
      if (t.type == Lex::TokenType::BraceL)
      {
        index++;
        t = getNextToken(tokens, index);

        result.braces.emplace();
        while (tokens[index].type != Lex::TokenType::BraceR)
          result.braces->push_back(parseNode(tokens, index));
        
        index++;
      }

      t = getNextToken(tokens, index);
    }

    if (parseOp)
    {
      while (t.type == Lex::TokenType::Operator)
      {
        Node l = result;
        
        index++;
        Node r = parseNode(tokens, index, false);

        result = Node();
        result.primary = parseValue(t);
        result.parens.emplace();
        result.parens->push_back(l);
        result.parens->push_back(r);

        t = tokens[index];
      }
    }

    return result;
  }

  std::vector<Node> parseNodes(const std::vector<Lex::Token> & tokens)
  {
    std::vector<Node> result;

    int i = 0;
    while (i < tokens.size())
    {
      auto node = Parse::parseNode(tokens, i);
      result.push_back(node);
    }

    return result;
  }
}