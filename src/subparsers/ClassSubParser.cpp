/**
 * @file ClassSubParser.cpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ClassSubParser.hpp"
#include "ArrayAccessNode.hpp"
#include "BaseNode.hpp"
#include "Exceptions.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"
#include "StructDefinitionObject.hpp"
#include "StructObject.hpp"


BaseNode *ClassSubParser::parseStruct()
{
    skip("struct");

    auto structTypeName = tokens().dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (equals(Token::Punctuation, "{") || equals(Token::Keyword, "extends"))
    {
        std::string structParentTypeName = "";

        if (equals(Token::Keyword, "extends"))
        {
            skip("extends");

            structParentTypeName = tokens().dequeue();
        }

        auto nodes = subparsers().block.parseDelimited("{", "}", ";", std::bind(&VariableSubParser::parseVariableDefinition, subparsers().variable));

        std::vector<AddVariableNode *> variableDefs;
        variableDefs.reserve(nodes.size());

        for (BaseNode *node : nodes)
        {
            variableDefs.push_back(reinterpret_cast<AddVariableNode *>(node));
        }

        return new StructDefinitionObject(structTypeName, structParentTypeName, variableDefs);
    }
    else
    {
        // Case: "struct STRUCT_TYPE_NAME & STRUCT_REF_INSTANCE_NAME = STRUCT_VARIABLE_NAME_TO_BIND"
        if (equals(Token::Operator, "&"))
        {
            return parent().subparsers().variable.parseReference(ObjectType::Struct);
        }

        auto structInstanceName = tokens().dequeue();

        return new StructObject(structTypeName, structInstanceName);
    }
}


BaseNode *ClassSubParser::parseClass()
{
    skip("class");

    auto classTypeName = tokens().dequeue();

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (equals(Token::Punctuation, "{") || equals(Token::Keyword, "extends"))
    {
        // *** Inheritance: class SomeClass(ParentClass) ***
        std::string classParentTypeName = "";

        if (equals(Token::Keyword, "extends"))
        {
            skip("extends");

            classParentTypeName = tokens().dequeue();
        }

        std::vector<BaseNode *> classBody = parent().subparsers().block.parseBraces();

        // Split-up into class variables and class methods:
        std::vector<AddVariableNode *> classVariables;
        std::vector<FunctionNode *> classMethods;

        for (BaseNode *node : classBody)
        {
            if (node->isNodeType(NodeType::AddVariable))
                classVariables.push_back(static_cast<AddVariableNode *>(node));
            else if (node->isNodeType(NodeType::Function))
                classMethods.push_back(static_cast<FunctionNode *>(node));
            else
                ThrowException("unexpected node type for class definition " + classTypeName);
        }

        return new ClassDefinitionObject(classTypeName, classParentTypeName, classVariables, classMethods);
    }
    else
    {
        // Case: "class CLASS_INSTANCE_NAME & CLASS_REF_NAME = CLASS_VARIABLE_NAME_TO_BIND"
        if (equals(Token::Operator, "&"))
        {
            return parent().subparsers().variable.parseReference(ObjectType::Class);
        }

        auto classInstanceName = tokens().dequeue();

        return new ClassObject(classTypeName, classInstanceName);
    }

    return nullptr;
}


BaseNode *ClassSubParser::parseStructAccessor(BaseNode *lastExpression)
{
    auto instanceName = lastExpression->castNode<LookupVariableNode>().name;
    delete lastExpression; // NB: ugly.

    skip(".");

    BaseNode *expression = parent().maybeFunctionCall(std::bind(&VariableSubParser::parseVariableName, &parent().subparsers().variable));

    if (expression->isNodeType(NodeType::FunctionCall)) // Method.
    {
        // NB: do NOT delete expression as owned by ClassMethodCallNode.
        return new ClassMethodCallNode(instanceName, reinterpret_cast<FunctionNode *>(expression));
    }
    else if (expression->isNodeType(NodeType::LookupVariable)) // Member variable.
    {
        std::string memberVariableName = expression->castNode<LookupVariableNode>().name;
        delete expression; // No longer required. Ugly.

        return NodeFactory::createStructAccessNode(instanceName, memberVariableName);
    }

    ThrowException("unexpected node type while accessing struct/class member");
}
