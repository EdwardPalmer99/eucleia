/**
 * @file VariableSubParser.hpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyObject.hpp"
#include "BaseNode.hpp"
#include "SubParser.hpp"

class FileParser;

class VariableSubParser : public SubParser
{
public:
    explicit VariableSubParser(FileParser &parser_) : SubParser(parser_) {}


    /* Parse: int/float/string/bool/array [variable name] */
    BaseNode::Ptr parseVariableDefinition();

    /**
     * Bind a reference to an already declared variable in this scope or a parent scope. We do not allow any unbound
     * references and once bound, references cannot be unbound. By default, we pass by value to functions, but the use
     * of a reference as in other languages such as C++ avoids copying.
     *
     * Parse: VARIABLE_TO_BIND_TO_TYPE & REFERENCE_NAME = VARIABLE_TO_BIND_TO;
     */
    BaseNode::Ptr parseReference(AnyObject::Type boundVariableType_);

    /* Parse: [variable name] */
    BaseNode::Ptr parseVariable();
};
