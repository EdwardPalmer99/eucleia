/**
 * @file SubParsers.cpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "SubParsers.hpp"

SubParsers::SubParsers(FileParser &parser_)
    : loop(parser_),
      controlFlow(parser_),
      block(parser_),
      unary(parser_),
      dataType(parser_),
      function(parser_),
      import(parser_),
      variable(parser_),
      classParser(parser_)
{
}
