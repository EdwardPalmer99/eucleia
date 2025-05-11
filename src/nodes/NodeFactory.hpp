/**
 * @file NodeFactory.hpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "BaseNode.hpp"
#include <string>

class AnyNode;

namespace NodeFactory
{

AnyNode *createBoolNode(bool state);

AnyNode *createIntNode(long value);

AnyNode *createStringNode(std::string value);

AnyNode *createFloatNode(double value);

AnyNode *createIfNode(BaseNode::Ptr condition, BaseNode::Ptr thenBranch, BaseNode::Ptr elseBranch = nullptr);

AnyNode *createForLoopNode(BaseNode::Ptr init, BaseNode::Ptr condition, BaseNode::Ptr update, BaseNode::Ptr body);

AnyNode *createWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body);

AnyNode *createDoWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body);

AnyNode *createBreakNode();

AnyNode *createReturnNode(BaseNode::Ptr returnNode = nullptr);

AnyNode *createNotNode(BaseNode::Ptr expression);

/* Evaluates a sequence of nodes */
AnyNode *createBlockNode(BaseNodeSharedPtrVector nodes);

} // namespace NodeFactory