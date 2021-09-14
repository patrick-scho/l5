#include "lex.h"
#include "parse.h"
#include "interp.h"
#include "log.h"
#include "print.h"

#include <iostream>
#include <fstream>

void printUsage(char *arg)
{
  std::cout << "Usage: " << arg << " <file>" << std::endl;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printUsage(argv[0]);
    return 1;
  }

  std::ifstream ifs(argv[1]);

  if (! ifs.good())
  {
    Log::error("Error opening file");
    return 1;
  }

  auto tokens = Lex::lex(ifs);

  ifs.close();

  //printTokens(tokens);

  auto nodes = Parse::parseNodes(tokens);
  for (auto node : nodes)
  {
    printNode(node);
    std::cout << "\n";
  }

  //auto result = interp::interp(ast);

  return 0;
}