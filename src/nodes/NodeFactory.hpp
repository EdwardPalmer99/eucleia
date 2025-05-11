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

/* Assign RHS to LHS */
AnyNode *createAssignNode(BaseNode *left, BaseNode *right);

AnyNode *createArrayNode(BaseNodeSharedPtrVector nodes);

/*
 * Evaluates a file treating it as one large program. Does NOT create an inner scope node in order to ensure that any
 * functions declared in this file will be added to the master file by using the same global scope - TODO: - think about this logic.
 */
AnyNode *createFileNode(BaseNodeSharedPtrVector nodes = {});

AnyNode *createPrefixIncrementNode(BaseNode *expression);

AnyNode *createPrefixDecrementNode(BaseNode *expression);

AnyNode *createNegationNode(BaseNode *expression);

} // namespace NodeFactory