/**
 * @file TopParser.hpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "FileNode.hpp"
#include "SingletonT.hpp"
#include <string>

/* Top-level parser for entire project (NB: wrap-in a singleton) */
class RootParser
{
public:
    /* Provide path to the project entry-point */
    FileNode *parse(std::string entryPointPath);
};
