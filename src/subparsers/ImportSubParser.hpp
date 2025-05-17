/**
 * @file ImportSubParser.hpp
 * @author Edward Palmer
 * @date 2025-05-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "AnyNode.hpp"
#include "BaseNode.hpp"
#include "SubParser.hpp"

class FileParser;

class ImportSubParser : public SubParser
{
public:
    explicit ImportSubParser(FileParser &parser) : SubParser(parser) {}

    BaseNode::Ptr parseImport();

    /*
     * Imports a file and its functions into this scope
     * Parse: import "path to file"
     */
    AnyNode::Ptr parseFileImport();

    /**
     * Imports functions from a stdlib as opposed to user-defined functions
     * Parse: import <library name>
     */
    AnyNode::Ptr parseModuleImport();
};