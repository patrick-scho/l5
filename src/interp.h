#pragma once

#include "parse.h"

#include <vector>
#include <functional>

namespace Interp
{
  struct Value
  {

  };
  struct Variable
  {
    std::string name, type;
  };
  struct Function
  {
    std::string name;
    std::vector<Variable> parameters;
    std::vector<Parse::Node *> body;
  };
  static std::vector<Function> functions;

  Variable getVariable(Parse::Node * node)
  {
    Variable result;
    result.name = std::get<Parse::Word>(node->primary).val;
    // if (node->secondary.has_value())
    //   result.type = std::get<Parse::Word>(node->secondary.value()).val;
    return result;
  }
  Function getFunction(Parse::Node * node)
  {
    
    Function f;
    // f.name = std::get<Parse::Word>(node->secondary.value()).val;
    for (auto p : node->parens.value())
    {
      f.parameters.push_back(getVariable(p.get()));
    }
    for (auto b : node->braces.value())
    {
      f.body.push_back(b.get());
    }
    return f;
  }

  void interp(Parse::Node * node)
  {
    if (std::holds_alternative<Parse::Word>(node->primary))
    {
      auto word = std::get<Parse::Word>(node->primary);
      if (word.val == "fn")
      {
        functions.push_back(getFunction(node));
      }
    }
  }
  void interp(const std::vector<std::shared_ptr<Parse::Node>> & nodes)
  {
    for (auto node : nodes)
    {
      interp(node.get());
    }
  }
}