#pragma once

#include "parse.h"
#include "lex.h"

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

void printNode(const Parse::Node & node)
{
  switch (node.type)
  {
  case Parse::NodeType::Root:
    for (auto c : node.children)
    {
      printNode(c);
      std::cout << std::endl;
    }
    break;
  case Parse::NodeType::FuncCall:
    std::cout << "FuncCall (" << node.info << "): ";
    for (auto c : node.children)
    {
      printNode(c);
      std::cout << ", ";
    }
    break;
  case Parse::NodeType::If:
    std::cout << "If ("; printNode(node.children[0]); std::cout << ") {\n";
    for (int i = 1; i < node.children.size(); i++)
    {
      printNode(node.children[i]);
      std::cout << std::endl;
    }
    std::cout << "}";
    break;
  case Parse::NodeType::Integer:
    std::cout << node.info;
    break;
  case Parse::NodeType::Operator:
    printNode(node.children[0]);
    std::cout << " " << node.info << " ";
    printNode(node.children[1]);
    break;
  }
}