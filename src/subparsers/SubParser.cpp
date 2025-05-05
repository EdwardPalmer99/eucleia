/**
 * @file SubParser.cpp
 * @author Edward Palmer
 * @date 2025-05-04
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "SubParser.hpp"
#include "FileParser.hpp"


SubParser::SubParser(FileParser &parser_) : _parser(parser_)
{
}


Tokens &SubParser::tokens()
{
    return _parser.tokens();
}