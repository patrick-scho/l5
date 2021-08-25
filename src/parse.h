#pragma once

#include "lex.h"

#include <vector>
#include <memory>

template<typename T>
using ptr = std::unique_ptr<T>;

namespace Parse
{
  enum class NodeType
  {
    Root, If, Operator, FuncCall, Integer
  };
  struct Node
  {
    NodeType type;

    Node *parent;
    std::vector<Node> children;

    std::string info;
  };

  Node parseNode(const std::vector<Lex::Token> & tokens, int & index);

  Node parseFuncCall(const std::vector<Lex::Token> & tokens, int & index)
  {
    Node result;
    result.type = NodeType::FuncCall;
    result.info = tokens[index].str;
    index += 2;

    if (tokens[index].type == Lex::TokenType::ParenR)
      return result;

    while (true)
    {
      result.children.push_back(
        parseNode(tokens, index)
      );
      if (tokens[index+1].type == Lex::TokenType::ParenR)
      {
        index++;
        break;
      }
      else
      {
        index += 2;
      }
    }

    return result;
  }

  Node parseIf(const std::vector<Lex::Token> & tokens, int & index)
  {
    Node result;
    result.type = NodeType::If;

    index += 2;

    result.children.push_back(
      parseNode(tokens, index)
    );

    index += 3;

    while (tokens[index].type != Lex::TokenType::BraceR)
    {
      result.children.push_back(
        parseNode(tokens, index)
      );
      index++;
    }

    return result;
  }

  Node parseInteger(const std::vector<Lex::Token> & tokens, int & index)
  {
    Node result;
    result.type = NodeType::Integer;
    result.info = tokens[index].str;
    return result;
  }

  Node parseNode(const std::vector<Lex::Token> & tokens, int & index)
  {
    Node result;

    auto & t = tokens[index];

    using Lex::TokenType;

    if (t.type == TokenType::Word)
    {
      if (t.str == "if")
      {
        result = parseIf(tokens, index);
      }
      else if (tokens[index+1].type == TokenType::ParenL)
      {
        result = parseFuncCall(tokens, index);
      }
    }
    else if (t.type == TokenType::Integer)
    {
      result = parseInteger(tokens, index);
    }

    while (tokens[index+1].type == TokenType::Operator)
    {
      Node tmp = result;
      result.type = NodeType::Operator;
      result.children.push_back(tmp);
      index++;
      result.info = tokens[index].str;
      index++;
      result.children.push_back(parseNode(tokens, index));
    }

    return result;
  }

  Node parseRoot(const std::vector<Lex::Token> & tokens)
  {
    Node result;
    result.type = NodeType::Root;
    result.parent = nullptr;

    for (int i = 0; i < tokens.size(); i++)
    {
      result.children.push_back(
        parseNode(tokens, i)
      );
    }

    return result;
  }
}