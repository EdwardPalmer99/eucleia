/**
 * @file TopParser.cpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "TopParser.hpp"
#include "FileParser.hpp"
#include "ParserData.hpp"

FileNode *RootParser::parse(std::string entryPointPath)
{
    /* Clear data for all imports */
    ParserData::instance().clearImports();

    /*
     * Create parser for entry-point file. If required, it will create additional
     * parsers for other imported files
     */
    return FileParser::parse(entryPointPath);
}
