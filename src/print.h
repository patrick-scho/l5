#pragma once

#include "parse.h"
#include "lex.h"

#include <variant>
#include <vector>
#include <iostream>
#include <iomanip>

void printTokens(const std::vector<Lex::Token> & tokens)
{
  for (auto t : tokens)
  {
    std::cout << std::left << std::setw(15);
    if (t.type == Lex::TokenType::ParenL)    std::cout << "ParenL";
    if (t.type == Lex::TokenType::ParenR)    std::cout << "ParenR";
    if (t.type == Lex::TokenType::BracketL)  std::cout << "BracketL";
    if (t.type == Lex::TokenType::BracketR)  std::cout << "BracketR";
    if (t.type == Lex::TokenType::BraceL)    std::cout << "BraceL";
    if (t.type == Lex::TokenType::BraceR)    std::cout << "BraceR";
    if (t.type == Lex::TokenType::Comma)     std::cout << "Comma";
    if (t.type == Lex::TokenType::Semicolon) std::cout << "Semicolon";
    if (t.type == Lex::TokenType::Word)      std::cout << "Word";
    if (t.type == Lex::TokenType::Integer)   std::cout << "Integer";
    if (t.type == Lex::TokenType::Operator)  std::cout << "Operator";
    std::cout << std::left << std::setw(10) <<
      t.str <<
      "(" << t.loc.row << "," << t.loc.col << ")" << std::endl;
  }
}

void printValue(const Parse::Value & value)
{
  bool printType = false;
  if (std::holds_alternative<Parse::Word>(value))
    std::cout << (printType ? "W: " : "") << std::get<Parse::Word>(value).val;
  if (std::holds_alternative<Parse::String>(value))
    std::cout << (printType ? "S: " : "") << std::get<Parse::String>(value).val;
  if (std::holds_alternative<Parse::Integer>(value))
    std::cout << (printType ? "I: " : "") << std::get<Parse::Integer>(value).val;
}

void printNode(const Parse::Node * node)
{
  if (std::holds_alternative<Parse::Operator>(node->primary))
  {
    auto primary = std::get<Parse::Operator>(node->primary);
    // infix operators
    if (node->parens.value().size() == 2)
    {
      // special case bracket
      if (primary.val == "[]")
      {
        printNode(node->parens.value()[0].get());
        std::cout << "[";
        printNode(node->parens.value()[1].get());
        std::cout << "]";
      }
      else
      {
        std::cout << "(";
        printNode(node->parens.value()[0].get());
        std::cout << " ";
        std::cout << primary.val;
        std::cout << " ";
        printNode(node->parens.value()[1].get());
        std::cout << ")";
      }
    }
    // prefix operators
    else if (node->parens.value().size() == 1)
    {
      // ignore parens
      if (primary.val != "()")
        std::cout << primary.val;
      printNode(node->parens.value()[0].get());
    }
    return;
  }
  printValue(node->primary);

  if (node->parens.has_value())
  {
    std::cout << "(";
    for (auto n : node->parens.value())
    {
      printNode(n.get());
      std::cout << ", ";
    }
    std::cout << ")";
  }
  if (node->braces.has_value())
  {
    std::cout << "{\n";
    for (auto n : node->braces.value())
    {
      printNode(n.get());
      std::cout << "\n";
    }
    std::cout << "}\n";
  }
}

