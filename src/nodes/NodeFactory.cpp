/**
 * @file NodeFactory.cpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "NodeFactory.hpp"
#include "AddVariableNode.hpp"
#include "ArrayObject.hpp"
#include "BaseObject.hpp"
#include "ClassObject.hpp"
#include "FloatObject.hpp"
#include "FunctionCallNode.hpp"
#include "IntObject.hpp"
#include "JumpPoints.hpp"
#include "LookupVariableNode.hpp"
#include "ObjectFactory.hpp"
#include "Scope.hpp"
#include "StringObject.hpp"
#include "StructObject.hpp"
#include <cassert>
#include <iostream>
#include <memory>

namespace NodeFactory
{

AnyNode::Ptr createBoolNode(bool state)
{
    return std::make_shared<AnyNode>(NodeType::Bool, [state](Scope &)
    {
        return ObjectFactory::allocate<BoolObject>(state);
    });
}

AnyNode::Ptr createIntNode(long value)
{
    return std::make_shared<AnyNode>(NodeType::Int, [value](Scope &)
    {
        return ObjectFactory::allocate<IntObject>(value);
    });
}

AnyNode::Ptr createStringNode(std::string value)
{
    return std::make_shared<AnyNode>(NodeType::String, [value = std::move(value)](Scope &)
    {
        return ObjectFactory::allocate<StringObject>(value);
    });
}

AnyNode::Ptr createFloatNode(double value)
{
    return std::make_shared<AnyNode>(NodeType::Float, [value](Scope &)
    {
        return ObjectFactory::allocate<FloatObject>(value);
    });
}

AnyNode::Ptr createIfNode(BaseNode::Ptr condition, BaseNode::Ptr thenBranch, BaseNode::Ptr elseBranch)
{
    return std::make_shared<AnyNode>(NodeType::If, [condition, thenBranch, elseBranch](Scope &scope) /* Use shared pointer to manage ownership */
    {
        if (condition->evaluate<BoolObject>(scope)->value())
            return thenBranch->evaluate(scope);
        else if (elseBranch)
            return elseBranch->evaluate(scope);
        else
            return BaseObject::Ptr();
    });
}

AnyNode::Ptr createForLoopNode(BaseNode::Ptr init, BaseNode::Ptr condition, BaseNode::Ptr update, BaseNode::Ptr body)
{
    return std::make_shared<AnyNode>(NodeType::ForLoop, [init, condition, update, body](Scope &scope)
    {
        // Initialization.
        Scope loopScope(scope); // Extend scope.

        (void)init->evaluate(loopScope);

        jmp_buf local;
        pushBreakJumpPoint(&local);

        // Add evaluation to forScope:
        if (setjmp(local) != 1)
        {
            for (;
                 condition->evaluate<BoolObject>(loopScope)->value();
                 update->evaluate(loopScope))
            {
                (void)body->evaluate(loopScope);
            }
        }

        popBreakJumpPoint();
        return nullptr;
    });
}


AnyNode::Ptr createWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body)
{
    return std::make_shared<AnyNode>(NodeType::While, [condition, body](Scope &scope)
    {
        // Set jump point for break statements.
        jmp_buf local;
        pushBreakJumpPoint(&local);

        if (setjmp(local) != 1)
        {
            Scope loopScope(scope); // Extend scope.

            while (condition->evaluate<BoolObject>(scope)->value()) /* Memory leak */
            {
                (void)body->evaluate(loopScope);
            }
        }

        // Restore original context.
        popBreakJumpPoint();

        return nullptr;
    });
}


AnyNode::Ptr createDoWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body)
{
    return std::make_shared<AnyNode>(NodeType::DoWhile, [condition, body](Scope &scope)
    {
        jmp_buf local;
        pushBreakJumpPoint(&local);

        if (setjmp(local) != 1)
        {
            Scope loopScope(scope); // Extend scope.

            do
            {
                (void)body->evaluate(loopScope);
            } while (condition->evaluate<BoolObject>(scope)->value()); /* NB: evaluate in outerscope (no access to loop scope)*/
        }

        // Restore original context.
        popBreakJumpPoint();

        return nullptr; // Return nothing.
    });
}


AnyNode::Ptr createBreakNode()
{
    return std::make_shared<AnyNode>(NodeType::Break, [](Scope &scope)
    {
        (void)scope;
        jumpToBreakJumpPoint(); /* Jump to last set point */

        return nullptr;
    });
}

AnyNode::Ptr createReturnNode(BaseNode::Ptr returnNode)
{
    return std::make_shared<AnyNode>(NodeType::Return, [returnNode](Scope &scope)
    {
        gEnvironmentContext.returnValue = nullptr;

        if (returnNode != nullptr) // i.e. return true;
        {
            gEnvironmentContext.returnValue = returnNode->evaluate(scope);
        }

        longjmp(*gEnvironmentContext.returnJumpPoint, 1);
        return nullptr;
    });
}


AnyNode::Ptr createNotNode(BaseNode::Ptr expression)
{
    return std::make_shared<AnyNode>(NodeType::Not, [expression](Scope &scope)
    {
        auto result = expression->evaluate<BoolObject>(scope);

        return ObjectFactory::allocate<BoolObject>(!result->value());
    });
}

AnyNode::Ptr createBlockNode(BaseNodePtrVector nodes)
{
    return std::make_shared<AnyNode>(NodeType::Block, [nodes = std::move(nodes)](Scope &scope)
    {
        /*
         * Create inner program scope for each block of statements. Good example is for a loop where the body of the
         * loop redeclares a variable. This should be okay
         */
        Scope blockScope(scope);

        for (const auto &node : nodes)
        {
            (void)node->evaluate(blockScope);
        }

        /* Any memory allocations cleared-up when we exit */
        return nullptr;
    });
}

AnyNode::Ptr createAssignNode(BaseNode::Ptr left, BaseNode::Ptr right)
{
    return std::make_shared<AnyNode>(NodeType::Assign, [left, right](Scope &scope)
    {
        // Setting array or struct values.
        if (left->isNodeType(NodeType::ArrayAccess) || left->isNodeType(NodeType::StructAccess))
        {
            auto &accessor = left->castNode<AnyPropertyNode>();

            *(accessor.evaluateNoClone(scope)) = *(right->evaluate(scope));
            return nullptr;
        }

        assert(left->isNodeType(NodeType::AddVariable) || left->isNodeType(NodeType::LookupVariable));

        // Case 1: AddVariableNode -> we create default init object, add to scope and return.
        // Case 2: LookupVariableNode -> we object defined in scope (not cloned!) - TODO: - think about whether we should clone it.
        BaseObject::Ptr objectLHS = left->evaluate(scope);

        // Object we want to assign to LHS.
        BaseObject::Ptr objectRHS = right->evaluate(scope);

        // Update directly. TODO: - We will need to implement this for some object types still.
        *objectLHS = *objectRHS;
        return nullptr;
    });
}


AnyNode::Ptr createArrayNode(BaseNodePtrVector nodes)
{
    // TODO: - could treat as references in array?
    return std::make_shared<AnyNode>(NodeType::Array, [nodes = std::move(nodes)](Scope &scope)
    {
        BaseObjectPtrVector evaluatedObjects;

        evaluatedObjects.reserve(nodes.size());

        for (auto &node : nodes)
        {
            evaluatedObjects.push_back(node->evaluate(scope));
        }

        return ObjectFactory::allocate<ArrayObject>(std::move(evaluatedObjects));
    });
}


AnyNode::Ptr createFileNode(BaseNodePtrVector nodes)
{
    return std::make_shared<AnyNode>(NodeType::File, [nodes](Scope &scope)
    {
        for (const auto &node : nodes)
        {
            (void)node->evaluate(scope);
        }

        return nullptr;
    });
}


AnyNode::Ptr createPrefixIncrementNode(BaseNode::Ptr expression)
{
    return std::make_shared<AnyNode>(NodeType::PrefixIncrement, [expression](Scope &scope)
    {
        // 1. Body should be an already-declared variable.
        assert(expression->isNodeType(NodeType::LookupVariable));

        // 2. Object associated with variable name in scope must be integer or float.
        auto bodyEvaluated = expression->evaluate(scope);
        if (bodyEvaluated->isObjectType<IntObject>())
        {
            ++(bodyEvaluated->castObject<IntObject>());
            return bodyEvaluated;
        }
        else if (bodyEvaluated->isObjectType<FloatObject>())
        {
            ++(bodyEvaluated->castObject<FloatObject>());
            return bodyEvaluated;
        }

        ThrowException("cannot use prefix operator on object of type");
    });
}


AnyNode::Ptr createPrefixDecrementNode(BaseNode::Ptr expression)
{
    return std::make_shared<AnyNode>(NodeType::PrefixDecrement, [expression](Scope &scope)
    {
        // 1. Body should be an already-declared variable.
        assert(expression->isNodeType(NodeType::LookupVariable));

        // 2. Object associated with variable name in scope must be integer or float.
        auto bodyEvaluated = expression->evaluate(scope);
        if (bodyEvaluated->isObjectType<IntObject>())
        {
            --(bodyEvaluated->castObject<IntObject>());
            return bodyEvaluated;
        }
        else if (bodyEvaluated->isObjectType<FloatObject>())
        {
            --(bodyEvaluated->castObject<FloatObject>());
            return bodyEvaluated;
        }

        ThrowException("cannot use prefix operator on object of type.");
    });
}


AnyNode::Ptr createNegationNode(BaseNode::Ptr expression)
{
    return std::make_shared<AnyNode>(NodeType::Negation, [expression](Scope &scope)
    {
        auto bodyEvaluated = expression->evaluate(scope);

        BaseObject::Ptr result{nullptr};

        if (bodyEvaluated->isObjectType<IntObject>())
            result = ObjectFactory::allocate<IntObject>(-bodyEvaluated->castObject<IntObject>());
        else if (bodyEvaluated->isObjectType<FloatObject>())
            result = ObjectFactory::allocate<FloatObject>(-bodyEvaluated->castObject<FloatObject>());
        else
            ThrowException("invalid object type");

        return result;
    });
}


AnyPropertyNode::Ptr createStructAccessNode(std::string structVarName, std::string memberVarName)
{
    auto evaluateNoClone = [structVarName, memberVarName](Scope &scope)
    {
        auto structObject = scope.getNamedObject<StructObject>(structVarName);
        return structObject->instanceScope().getNamedObject(memberVarName);
    };

    auto evaluate = [evaluateNoClone](Scope &scope)
    {
        BaseObject::Ptr currentObject = evaluateNoClone(scope);
        return currentObject->clone();
    };


    return std::make_shared<AnyPropertyNode>(NodeType::StructAccess, std::move(evaluate), std::move(evaluateNoClone));
}


AnyPropertyNode::Ptr createArrayAccessNode(BaseNode::Ptr arrayLookupNode, BaseNode::Ptr arrayIndexNode)
{
    assert(arrayLookupNode->isNodeType(NodeType::LookupVariable));

    auto evaluateNoClone = [arrayLookupNode, arrayIndexNode](Scope &scope)
    {
        // Lookup in array.
        auto arrayObj = arrayLookupNode->evaluate(scope)->castObject<ArrayObject>();

        return arrayObj[arrayIndexNode->evaluateObject<IntObject::Type>(scope)];
    };

    auto evaluate = [evaluateNoClone](Scope &scope)
    {
        BaseObject::Ptr currentObject = evaluateNoClone(scope);
        return currentObject->clone();
    };

    return std::make_shared<AnyPropertyNode>(NodeType::ArrayAccess, std::move(evaluate), std::move(evaluateNoClone));
}


AnyNode::Ptr createModuleNode(std::string moduleName, std::vector<ModuleFunctionPair> moduleFunctions)
{
    return std::make_shared<AnyNode>(NodeType::Module, [moduleName = std::move(moduleName), moduleFunctions = std::move(moduleFunctions)](Scope &scope)
    {
        for (auto &it : moduleFunctions) /* Add to scope */
        {
            auto object = ObjectFactory::allocate<ModuleFunctionObject>(it.second);
            scope.linkObject(it.first, object);
        }

        return nullptr;
    });
}

AnyNode::Ptr createClassMethodCallNode(std::string instanceName, FunctionCallNode::Ptr methodCallNode)
{
    return std::make_shared<AnyNode>(NodeType::ClassMethodCall, [instanceName = std::move(instanceName),
                                                                 methodCallNode](Scope &scope)
    {
        auto thisObject = scope.getNamedObject<ClassObject>(instanceName);

        // Important: to correctly evaluate the method, we need to add a parent scope
        // for the class instance temporarily each time we evaluate so function has
        // access to stuff created outside class.
        thisObject->instanceScope().setParentScope(&scope);

        /* But evaluate in class' instance scope */
        BaseObject::Ptr result = methodCallNode->evaluate(thisObject->instanceScope());

        // Set back to avoid problems if we forget to reset it in future.
        thisObject->instanceScope().setParentScope(nullptr);

        return result;
    });
}


} // namespace NodeFactory
