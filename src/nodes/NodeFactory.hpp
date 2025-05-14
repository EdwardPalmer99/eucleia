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
#include "LibraryFunctionObject.hpp"
#include <string>
#include <vector>

class AnyNode;
class AnyPropertyNode;
class FunctionCallNode;

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

AnyPropertyNode *createStructAccessNode(std::string structVariableName, std::string structMemberName);

AnyPropertyNode *createArrayAccessNode(BaseNode *arrayLookupNode, BaseNode *arrayIndexNode);

AnyNode *createModuleNode(std::string moduleName = "", std::vector<ModuleFunctionPair> moduleFunctionPairs = {});

AnyNode *createClassMethodCallNode(std::string instanceName, FunctionCallNode *methodCallNode);

} // namespace NodeFactory


/********************
TODO: - interesting performance enhancements

1. Try to have as few Node types as possible (as I'm currently doing)
2. Then overload new/delete operators
3. Store Node types in a flat array
4. Can use a ThreadPool to build nodes (don't need to wait for FileNode to create --> since it's not referenced. Just
   know where we're putting the Node in the flat array and any of its children)

**************************** */