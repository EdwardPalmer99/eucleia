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
#include <memory>

BaseNode::Ptr ClassSubParser::parseClass()
{
    skip("class");

    auto classTypeName = tokens().dequeue();

    /* Insert to keep track of class definitions */
    _parsedClassDefinitions.insert(classTypeName);

    // Do we have a '{' token next? If we do then it is definition of new struct.
    if (equals(Token::Punctuation, "{") || equals(Token::Punctuation, ":"))
    {
        // *** Inheritance: class SomeClass(ParentClass) ***
        std::string classParentTypeName = "";

        if (equals(Token::Punctuation, ":"))
        {
            skip(":");

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

    ThrowException("Failed to parse class definition");
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
