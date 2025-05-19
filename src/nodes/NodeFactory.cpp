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
#include "ClassNode.hpp"
#include "FunctionCallNode.hpp"
#include "JumpPoints.hpp"
#include "LookupVariableNode.hpp"
#include "ObjectFactory.hpp"
#include "Scope.hpp"
#include "StructNode.hpp"
#include <cassert>
#include <iostream>
#include <memory>

namespace NodeFactory
{

AnyNode::Ptr createCastNode(BaseNode::Ptr expression, AnyObject::Type castToType)
{
    auto isCastable = [](AnyObject::Type castToType) -> bool
    {
        return (castToType == AnyObject::Int || castToType == AnyObject::Float);
    };

    if (!isCastable(castToType))
    {
        ThrowException("Cannot create cast node. Unsupported cast type!");
    }

    return std::make_shared<AnyNode>(NodeType::Cast, [isCastable, expression, castToType](Scope &scope)
    {
        auto evaluatedObject = expression->evaluate(scope);
        if (!isCastable(evaluatedObject->getType()))
        {
            ThrowException("Unsupported cast type!");
        }

        if (evaluatedObject->getType() == castToType) /* Nothing to do */
        {
            return evaluatedObject;
        }

        if (castToType == AnyObject::Int)
        {
            long value = (long)expression->evaluate(scope)->getValue<double>();
            return ObjectFactory::allocate(value);
        }
        else
        {
            double value = (double)expression->evaluate(scope)->getValue<long>();
            return ObjectFactory::allocate(value);
        }
    });
}

AnyNode::Ptr createBoolNode(bool state)
{
    return std::make_shared<AnyNode>(NodeType::Bool, [state](Scope &)
    {
        return ObjectFactory::allocate(state);
    });
}

AnyNode::Ptr createIntNode(long value)
{
    return std::make_shared<AnyNode>(NodeType::Int, [value](Scope &)
    {
        return ObjectFactory::allocate(value);
    });
}

AnyNode::Ptr createStringNode(std::string value)
{
    return std::make_shared<AnyNode>(NodeType::String, [value = std::move(value)](Scope &)
    {
        return ObjectFactory::allocate(value);
    });
}

AnyNode::Ptr createFloatNode(double value)
{
    return std::make_shared<AnyNode>(NodeType::Float, [value](Scope &)
    {
        return ObjectFactory::allocate(value);
    });
}

AnyNode::Ptr createIfNode(BaseNode::Ptr condition, BaseNode::Ptr thenBranch, BaseNode::Ptr elseBranch)
{
    return std::make_shared<AnyNode>(NodeType::If, [condition, thenBranch, elseBranch](Scope &scope) /* Use shared pointer to manage ownership */
    {
        if (condition->evaluate(scope)->getValue<bool>())
            return thenBranch->evaluate(scope);
        else if (elseBranch)
            return elseBranch->evaluate(scope);
        else
            return AnyObject::Ptr();
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
                 condition->evaluate(loopScope)->getValue<bool>();
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

            while (condition->evaluate(scope)->getValue<bool>())
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
            } while (condition->evaluate(scope)->getValue<bool>()); /* NB: evaluate in outerscope (no access to loop scope) */
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
        AnyObject::Ptr result = expression->evaluate(scope);

        return ObjectFactory::allocate(!result->getValue<bool>());
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
        AnyObject::Ptr objectLHS = left->evaluate(scope);

        // Object we want to assign to LHS.
        AnyObject::Ptr objectRHS = right->evaluate(scope);

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
        AnyObject::Vector evaluatedObjects;

        evaluatedObjects.reserve(nodes.size());

        for (auto &node : nodes)
        {
            evaluatedObjects.push_back(node->evaluate(scope));
        }

        return ObjectFactory::allocate(std::move(evaluatedObjects));
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
        if (bodyEvaluated->isType(AnyObject::Int))
        {
            ++(bodyEvaluated->getValue<long>());
            return bodyEvaluated;
        }
        else if (bodyEvaluated->isType(AnyObject::Float))
        {
            ++(bodyEvaluated->getValue<double>());
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
        if (bodyEvaluated->isType(AnyObject::Int))
        {
            --(bodyEvaluated->getValue<long>());
            return bodyEvaluated;
        }
        else if (bodyEvaluated->isType(AnyObject::Float))
        {
            --(bodyEvaluated->getValue<double>());
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

        AnyObject::Ptr result{nullptr};

        if (bodyEvaluated->isType(AnyObject::Int))
            result = ObjectFactory::allocate(-bodyEvaluated->getValue<long>());
        else if (bodyEvaluated->isType(AnyObject::Float))
            result = ObjectFactory::allocate(-bodyEvaluated->getValue<double>());
        else
            ThrowException("invalid object type");

        return result;
    });
}


AnyPropertyNode::Ptr createStructAccessNode(std::string structVarName, std::string memberVarName)
{
    auto evaluateNoClone = [structVarName, memberVarName](Scope &scope)
    {
        auto theObject = scope.getNamedObject(structVarName);

        auto theStructObject = std::static_pointer_cast<StructNode>(theObject->getValue<BaseNode::Ptr>());

        return theStructObject->instanceScope().getNamedObject(memberVarName);
    };

    auto evaluate = [evaluateNoClone](Scope &scope)
    {
        auto currentObject = evaluateNoClone(scope);
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
        auto theArrayObject = arrayLookupNode->evaluate(scope); /* Careful with references! */

        auto &arrayObj = theArrayObject->getValue<AnyObject::Vector>();
        auto index = arrayIndexNode->evaluate(scope)->getValue<long>();

        if (index < 0 || index >= (long)arrayObj.size())
            ThrowException("Array index [" + std::to_string(index) + "] is out of bounds!");

        return arrayObj[index];
    };

    auto evaluate = [evaluateNoClone](Scope &scope)
    {
        AnyObject::Ptr currentObject = evaluateNoClone(scope);
        return currentObject->clone();
    };

    return std::make_shared<AnyPropertyNode>(NodeType::ArrayAccess, std::move(evaluate), std::move(evaluateNoClone));
}


AnyNode::Ptr createModuleNode(std::string moduleName, ModuleFunctor::Definitions moduleFunctions)
{
    return std::make_shared<AnyNode>(NodeType::Module, [moduleName = std::move(moduleName), moduleFunctions = std::move(moduleFunctions)](Scope &scope)
    {
        for (auto &it : moduleFunctions) /* Add to scope */
        {
            auto object = ObjectFactory::allocate(ModuleFunctor(it.second));
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
        auto anyObject = scope.getNamedObject(instanceName);

        auto thisObject = std::static_pointer_cast<ClassNode>(anyObject->getValue<BaseNode::Ptr>());

        // Important: to correctly evaluate the method, we need to add a parent scope
        // for the class instance temporarily each time we evaluate so function has
        // access to stuff created outside class.
        thisObject->instanceScope().setParentScope(&scope);

        /* But evaluate in class' instance scope */
        AnyObject::Ptr result = methodCallNode->evaluate(thisObject->instanceScope());

        // Set back to avoid problems if we forget to reset it in future.
        thisObject->instanceScope().setParentScope(nullptr);

        return result;
    });
}


} // namespace NodeFactory
