/**
 * @file NodeFactory.hpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "AnyNode.hpp"
#include "BaseNode.hpp"
#include "FunctionCallNode.hpp"
#include "ModuleFunctionObject.hpp"
#include <string>
#include <vector>


namespace NodeFactory
{
AnyNode::Ptr createBoolNode(bool state);

AnyNode::Ptr createIntNode(long value);

AnyNode::Ptr createStringNode(std::string value);

AnyNode::Ptr createFloatNode(double value);

AnyNode::Ptr createIfNode(BaseNode::Ptr condition, BaseNode::Ptr thenBranch, BaseNode::Ptr elseBranch = nullptr);

AnyNode::Ptr createForLoopNode(BaseNode::Ptr init, BaseNode::Ptr condition, BaseNode::Ptr update, BaseNode::Ptr body);

AnyNode::Ptr createWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body);

AnyNode::Ptr createDoWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body);

AnyNode::Ptr createBreakNode();

AnyNode::Ptr createReturnNode(BaseNode::Ptr returnNode = nullptr);

AnyNode::Ptr createNotNode(BaseNode::Ptr expression);

/* Evaluates a sequence of nodes */
AnyNode::Ptr createBlockNode(BaseNodePtrVector nodes);

/* Assign RHS to LHS */
AnyNode::Ptr createAssignNode(BaseNode::Ptr left, BaseNode::Ptr right);

AnyNode::Ptr createArrayNode(BaseNodePtrVector nodes);

/*
 * Evaluates a file treating it as one large program. Does NOT create an inner scope node in order to ensure that any
 * functions declared in this file will be added to the master file by using the same global scope - TODO: - think about this logic.
 */
AnyNode::Ptr createFileNode(BaseNodePtrVector nodes = {});

AnyNode::Ptr createPrefixIncrementNode(BaseNode::Ptr expression);

AnyNode::Ptr createPrefixDecrementNode(BaseNode::Ptr expression);

AnyNode::Ptr createNegationNode(BaseNode::Ptr expression);

AnyPropertyNode::Ptr createStructAccessNode(std::string structVariableName, std::string structMemberName);

AnyPropertyNode::Ptr createArrayAccessNode(BaseNode::Ptr arrayLookupNode, BaseNode::Ptr arrayIndexNode);

AnyNode::Ptr createModuleNode(std::string moduleName = "", std::vector<ModuleFunctionPair> moduleFunctionPairs = {});

AnyNode::Ptr createClassMethodCallNode(std::string instanceName, FunctionCallNode::Ptr methodCallNode);

} // namespace NodeFactory


/********************
TODO: - interesting performance enhancements

1. Try to have as few Node types as possible (as I'm currently doing)
2. Then overload new/delete operators
3. Store Node types in a flat array
4. Can use a ThreadPool to build nodes (don't need to wait for FileNode to create --> since it's not referenced. Just
   know where we're putting the Node in the flat array and any of its children)

**************************** */