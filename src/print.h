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
    if (t.type == Lex::TokenType::Colon)     std::cout << "Colon";
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
  if (std::holds_alternative<Parse::Word>(value))
    std::cout << "W: " << std::get<Parse::Word>(value).val;
  if (std::holds_alternative<Parse::Operator>(value))
    std::cout << "O: " << std::get<Parse::Operator>(value).val;
  if (std::holds_alternative<Parse::String>(value))
    std::cout << "S: " << std::get<Parse::String>(value).val;
  if (std::holds_alternative<Parse::Integer>(value))
    std::cout << "I: " << std::get<Parse::Integer>(value).val;
}

void printNode(const Parse::Node * node)
{
  printValue(node->primary);

  if (node->secondary.has_value())
  {
    std::cout << "[";
    printValue(node->secondary.value());
    std::cout << "]";
  }

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

