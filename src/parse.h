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
    Node * parent;

    Value primary;

    std::optional<std::vector<std::shared_ptr<Node>>> parens;
    std::optional<std::vector<std::shared_ptr<Node>>> braces;
  };

  Lex::Token getNextToken(const std::vector<Lex::Token> & tokens, int & index)
  {
    if (index >= tokens.size())
      return Lex::Token { Lex::TokenType::None };
    return tokens[index];
  }




  static std::vector<std::string> operatorPrecedence = {
    "+", "*"
  };

  int getOperatorPrecedence(const std::string & op)
  {
    for (int i = 0; i < operatorPrecedence.size(); i++)
    {
      if (operatorPrecedence[i] == op)
        return i;
    }
    return -1;
  }

  void checkOperatorPrecedence(std::shared_ptr<Node> node)
  {
    int p1 = getOperatorPrecedence(std::get<Parse::Operator>(node->parens.value()[0]->primary).val);
    int p2 = getOperatorPrecedence(std::get<Parse::Operator>(node->primary).val);

    if (p1 < p2)
    {
      auto tmp = node->primary;
      node->primary = node->parens.value()[0]->primary;
      node->parens.value()[0]->primary = tmp;

      auto tmp1 = node->parens.value()[0];
      node->parens.value()[0] = node->parens.value()[1];
      node->parens.value()[1] = tmp1;

      auto tmp2 = node->parens.value()[0];
      node->parens.value()[0] = node->parens.value()[1]->parens.value()[0];
      node->parens.value()[1]->parens.value()[0] = node->parens.value()[1]->parens.value()[1];
      node->parens.value()[1]->parens.value()[1] = tmp2;
    }
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

  std::shared_ptr<Node> parseNode(const std::vector<Lex::Token> & tokens, int & index, Node * parent, bool parseOp = true)
  {
    auto result = std::make_shared<Node>();
    result->parent = parent;

    auto t = tokens[index];

    if (t.type == Lex::TokenType::ParenL)
    {
      index++;
      result = parseNode(tokens, index, parent);
      if (tokens[index].type != Lex::TokenType::ParenR)
        Log::error("Expected ) at %d, %d", tokens[index].loc.row, tokens[index].loc.col);
      index++;
      t = getNextToken(tokens, index);
    }
    else if (t.type == Lex::TokenType::Operator)
    {
      index++;
      result->primary = Parse::Operator { t.str };
      result->parens.emplace();
      result->parens->push_back(parseNode(tokens, index, parent));
      t = getNextToken(tokens, index);
    }
    else
    {
      result->primary = parseValue(t);

      index++;
      t = getNextToken(tokens, index);
      
      if (t.type == Lex::TokenType::ParenL)
      {
        index++;
        t = getNextToken(tokens, index);
    
        result->parens.emplace();
        while (tokens[index].type != Lex::TokenType::ParenR)
          result->parens->push_back(parseNode(tokens, index, result.get()));
    
        index++;
      }

      t = getNextToken(tokens, index);
    
      if (t.type == Lex::TokenType::BraceL)
      {
        index++;
        t = getNextToken(tokens, index);

        result->braces.emplace();
        while (tokens[index].type != Lex::TokenType::BraceR)
          result->braces->push_back(parseNode(tokens, index, result.get()));
        
        index++;
      }

      t = getNextToken(tokens, index);
    }

    if (parseOp)
    {
      while (t.type == Lex::TokenType::Operator ||
             t.type == Lex::TokenType::BracketL)
      {
        if (t.type == Lex::TokenType::Operator)
        {
          Node * oldParent = result->parent;

          auto l = result;
          
          index++;
          std::shared_ptr<Node> r;
          if (t.str == "=")
            r = parseNode(tokens, index, parent, true);
          else
            r = parseNode(tokens, index, parent, false);

          result = std::make_shared<Node>();
          result->parent = oldParent;
          l->parent = result.get();
          r->parent = result.get();
          result->primary = parseValue(t);
          result->parens.emplace();
          result->parens->push_back(l);
          result->parens->push_back(r);

          if (std::holds_alternative<Parse::Operator>(l->primary) && l->parens.value().size() == 2)
          {
            checkOperatorPrecedence(result);
          }

          t = tokens[index];
        }
        else if (t.type == Lex::TokenType::BracketL)
        {
          Node * oldParent = result->parent;

          auto l = result;
          
          index++;
          std::shared_ptr<Node> r = parseNode(tokens, index, parent);

          result = std::make_shared<Node>();
          result->parent = oldParent;
          l->parent = result.get();
          r->parent = result.get();
          result->primary = Parse::Operator { "[]" };
          result->parens.emplace();
          result->parens->push_back(l);
          result->parens->push_back(r);

          index++;

          t = tokens[index];
        }
      }
      if (t.type == Lex::TokenType::Semicolon)
      {
        index++;
      }
    }

    return result;
  }

  std::vector<std::shared_ptr<Node>> parseNodes(const std::vector<Lex::Token> & tokens)
  {
    std::vector<std::shared_ptr<Node>> result;

    int i = 0;
    while (i < tokens.size())
    {
      auto node = Parse::parseNode(tokens, i, nullptr);
      result.push_back(node);
    }

    return result;
  }
}