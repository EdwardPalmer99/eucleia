/**
 * @file ImportSubParser.hpp
 * @author Edward Palmer
 * @date 2025-05-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "SubParser.hpp"

class FileParser;
class ModuleNode;
class FileNode;
class BaseNode;


class ImportSubParser : public SubParser
{
public:
    explicit ImportSubParser(FileParser &parser) : SubParser(parser) {}

    BaseNode *parseImport();

    /*
     * Imports a file and its functions into this scope
     * Parse: import "path to file"
     */
    FileNode *parseFileImport();

    /**
     * Imports functions from a stdlib as opposed to user-defined functions
     * Parse: import <library name>
     */
    ModuleNode *parseLibraryImport();
};