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

class FileParser;
class BaseNode;


class ClassSubParser : public SubParser
{
public:
    explicit ClassSubParser(FileParser &parser_) : SubParser(parser_) {}

    /**
     * Case 1: parse definition of a new structure:
     *
     * struct SomeStruct
     * {
     *      int i;
     *      float f;
     *      string s;
     * };
     *
     * Case 2: parse instance of structure.
     *
     * struct SomeStruct aStruct;
     */
    BaseNode *parseStruct();

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
     *
     *
     * Case 2: parse instance of the class:
     *
     * class SomeClass aClass;
     */
    BaseNode *parseClass();

    /**
     * Example:
     *
     * aStruct.i --> returns int object stored in struct instance.
     */
    BaseNode *parseStructAccessor(BaseNode *lastExpression);
};