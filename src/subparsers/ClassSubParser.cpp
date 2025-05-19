/**
 * @file ClassSubParser.cpp
 * @author Edward Palmer
 * @date 2025-05-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ClassSubParser.hpp"
#include "BaseNode.hpp"
#include "Exceptions.hpp"
#include "FileParser.hpp"
#include "NodeFactory.hpp"
#include "StructDefinitionNode.hpp"
#include "StructNode.hpp"
#include <memory>


BaseNode::Ptr ClassSubParser::parseStruct()
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

        std::vector<AddVariableNode::Ptr> variableDefs;
        variableDefs.reserve(nodes.size());

        for (BaseNode::Ptr node : nodes)
        {
            variableDefs.push_back(std::reinterpret_pointer_cast<AddVariableNode>(node));
        }

        return std::make_shared<StructDefinitionNode>(structTypeName, structParentTypeName, variableDefs);
    }
    else
    {
        // Case: "struct STRUCT_TYPE_NAME & STRUCT_REF_INSTANCE_NAME = STRUCT_VARIABLE_NAME_TO_BIND"
        if (equals(Token::Operator, "&"))
        {
            return parent().subparsers().variable.parseReference(AnyObject::Struct);
        }

        auto structInstanceName = tokens().dequeue();

        return std::make_shared<StructNode>(structTypeName, structInstanceName);
    }
}


BaseNode::Ptr ClassSubParser::parseClass()
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

        std::vector<BaseNode::Ptr> classBody = parent().subparsers().block.parseBraces();

        // Split-up into class variables and class methods:
        std::vector<AddVariableNode::Ptr> classVariables;
        std::vector<FunctionNode::Ptr> classMethods;

        for (BaseNode::Ptr node : classBody)
        {
            if (node->isNodeType(NodeType::AddVariable))
                classVariables.push_back(std::reinterpret_pointer_cast<AddVariableNode>(node));
            else if (node->isNodeType(NodeType::Function))
                classMethods.push_back(std::reinterpret_pointer_cast<FunctionNode>(node));
            else
                ThrowException("unexpected node type for class definition " + classTypeName);
        }

        return std::make_shared<ClassDefinitionNode>(classTypeName, classParentTypeName, classVariables, classMethods);
    }
    else
    {
        // Case: "class CLASS_INSTANCE_NAME & CLASS_REF_NAME = CLASS_VARIABLE_NAME_TO_BIND"
        if (equals(Token::Operator, "&"))
        {
            return parent().subparsers().variable.parseReference(AnyObject::Class);
        }

        auto classInstanceName = tokens().dequeue();

        return std::make_shared<ClassNode>(classTypeName, classInstanceName);
    }

    return nullptr;
}


BaseNode::Ptr ClassSubParser::parseStructAccessor(BaseNode::Ptr lastExpression)
{
    auto instanceName = lastExpression->castNode<LookupVariableNode>().name();

    skip(".");

    BaseNode::Ptr expression = parent().maybeFunctionCall(std::bind(&VariableSubParser::parseVariable, &parent().subparsers().variable));

    if (expression->isNodeType(NodeType::FunctionCall)) // Method.
    {
        return NodeFactory::createClassMethodCallNode(std::move(instanceName), std::static_pointer_cast<FunctionCallNode>(expression));
    }
    else if (expression->isNodeType(NodeType::LookupVariable)) // Member variable.
    {
        std::string memberVariableName = expression->castNode<LookupVariableNode>().name();

        return NodeFactory::createStructAccessNode(std::move(instanceName), memberVariableName);
    }

    ThrowException("unexpected node type while accessing struct/class member");
}
