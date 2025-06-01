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
     *      int i;å
     *
     *      func someFunc(int a)
            {
                i = i + 1;
                print("Hello");
                return i;
            }
     * };
     *
     *
     * Case 2: parse instance of the class:
     *
     * class SomeClass aClass;
     */
    BaseNode::Ptr parseClass();

    /**
     * Example:
     *
     * aStruct.i --> returns int object stored in struct instance.
     */
    BaseNode::Ptr parseStructAccessor(BaseNode::Ptr lastExpression);
};