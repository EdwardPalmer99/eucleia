/**
 * @file ClassSubParser.hpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "SubParser.hpp"
#include <string>
#include <unordered_set>

class FileParser;
class BaseNode;


class ClassSubParser : public SubParser
{
public:
    explicit ClassSubParser(FileParser &parser_) : SubParser(parser_) {}

    /**
     * Case 1: parse definition of a new class:
     *
     * class SomeClass
     * {
     *      int i;
     *
     *      func someFunc(int a)
            {
                i = i + 1;
                print("Hello");
                return i;
            }
     * };
     */
    BaseNode::Ptr parseClass();

    /**
     * Example:
     *
     * aStruct.i --> returns int object stored in struct instance.
     */
    BaseNode::Ptr parseStructAccessor(BaseNode::Ptr lastExpression);

    inline bool isParsedClassDefinition(const std::string &name) const;

private:
    using ClassDefinitionSet = std::unordered_set<std::string>;

    ClassDefinitionSet _parsedClassDefinitions;
};


bool ClassSubParser::isParsedClassDefinition(const std::string &name) const
{
    return _parsedClassDefinitions.count(name);
}
