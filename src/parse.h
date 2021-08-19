#pragma once

#include "lex.h"

namespace Parse
{
  enum class NodeType
  {

  };
  struct Node
  {
    NodeType type;
    Node *parent;


  };

  Node parse(const std::vector<Lex::Token> & tokens)
  {
    Node result;

    return result;
  }
}