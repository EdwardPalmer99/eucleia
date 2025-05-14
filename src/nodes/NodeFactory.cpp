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
#include "AnyNode.hpp"
#include "ArrayObject.hpp"
#include "BaseObject.hpp"
#include "ClassObject.hpp"
#include "ExpressionScope.hpp"
#include "FloatObject.hpp"
#include "FunctionCallNode.hpp"
#include "IntObject.hpp"
#include "JumpPoints.hpp"
#include "LookupVariableNode.hpp"
#include "Scope.hpp"
#include "StringObject.hpp"
#include "StructObject.hpp"
#include <cassert>
#include <memory>

namespace NodeFactory
{

AnyNode *createBoolNode(bool state)
{
    return new AnyNode(NodeType::Bool, [state](Scope &scope)
    {
        return scope.createManagedObject<BoolObject>(state);
    });
}

AnyNode *createIntNode(long value)
{
    return new AnyNode(NodeType::Int, [value](Scope &scope)
    {
        return scope.createManagedObject<IntObject>(value);
    });
}

AnyNode *createStringNode(std::string value)
{
    return new AnyNode(NodeType::String, [value = std::move(value)](Scope &scope)
    {
        return scope.createManagedObject<StringObject>(value);
    });
}

AnyNode *createFloatNode(double value)
{
    return new AnyNode(NodeType::Float, [value](Scope &scope)
    {
        return scope.createManagedObject<FloatObject>(value);
    });
}

AnyNode *createIfNode(BaseNode::Ptr condition, BaseNode::Ptr thenBranch, BaseNode::Ptr elseBranch)
{
    return new AnyNode(NodeType::If, [condition, thenBranch, elseBranch](Scope &scope) /* Use shared pointer to manage ownership */
    {                                                                                  /* Lifetime of owned nodes will end when AnyNode's destructor is called */
      if (evaluateExpression<BoolObject::Type>(condition.get(), scope))
          return thenBranch->evaluate(scope);
      else if (elseBranch)
          return elseBranch->evaluate(scope);
      else
          return static_cast<BaseObject *>(nullptr);
    });
}

AnyNode *createForLoopNode(BaseNode::Ptr init, BaseNode::Ptr condition, BaseNode::Ptr update, BaseNode::Ptr body)
{
    return new AnyNode(NodeType::ForLoop, [init, condition, update, body](Scope &scope)
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
                 evaluateExpression<BoolObject::Type>(condition.get(), loopScope); // TODO: - not very efficient repeatedly recalculating...
                 update->evaluate(loopScope))
            {
                (void)body->evaluate(loopScope);
            }
        }

        popBreakJumpPoint();
        return nullptr;
    });
}


AnyNode *createWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body)
{
    return new AnyNode(NodeType::While, [condition, body](Scope &scope)
    {
        // Set jump point for break statements.
        jmp_buf local;
        pushBreakJumpPoint(&local);

        if (setjmp(local) != 1)
        {
            Scope loopScope(scope); // Extend scope.

            while (evaluateExpression<BoolObject::Type>(condition.get(), scope))
            {
                (void)body->evaluate(loopScope);
            }
        }

        // Restore original context.
        popBreakJumpPoint();

        return nullptr;
    });
}


AnyNode *createDoWhileLoopNode(BaseNode::Ptr condition, BaseNode::Ptr body)
{
    return new AnyNode(NodeType::DoWhile, [condition, body](Scope &scope)
    {
        jmp_buf local;
        pushBreakJumpPoint(&local);

        if (setjmp(local) != 1)
        {
            Scope loopScope(scope); // Extend scope.

            do
            {
                (void)body->evaluate(loopScope);
            } while (evaluateExpression<BoolObject::Type>(condition.get(), scope));
        }

        // Restore original context.
        popBreakJumpPoint();

        return nullptr; // Return nothing.
    });
}


AnyNode *createBreakNode()
{
    return new AnyNode(NodeType::Break, [](Scope &scope)
    {
        (void)scope;
        jumpToBreakJumpPoint(); /* Jump to last set point */

        return nullptr;
    });
}

AnyNode *createReturnNode(BaseNode::Ptr returnNode)
{
    return new AnyNode(NodeType::Return, [returnNode](Scope &scope)
    {
        gEnvironmentContext.returnValue = nullptr;

        if (returnNode != nullptr) // i.e. return true;
        {
            // Evaluate in function scope (need all local variables, etc.).
            BaseObject *tmpResult = returnNode->evaluate(scope);
            if (tmpResult != nullptr)
            {
                // TODO: - instead, remove this from the scope and copy into parent scope instead of cloning.

                // Get the scope in which the function was called. We copy the object to this scope.
                // This is because the function scope will destroy the return object as soon as its
                // destructor is called and we need this object to persist until parent scope destructor called.
                assert(scope.parentScope() != nullptr);

                BaseObject *result = scope.parentScope()->cloneObject(tmpResult);
                gEnvironmentContext.returnValue = result;
            }
        }

        longjmp(*gEnvironmentContext.returnJumpPoint, 1);
        return nullptr;
    });
}


AnyNode *createNotNode(BaseNode::Ptr expression)
{
    return new AnyNode(NodeType::Not, [expression](Scope &scope)
    {
        auto result = expression->evaluate<BoolObject>(scope);

        return scope.createManagedObject<BoolObject>(!*result); // TODO: - seems really inefficient
    });
}

AnyNode *createBlockNode(BaseNodeSharedPtrVector nodes)
{
    return new AnyNode(NodeType::Block, [nodes = std::move(nodes)](Scope &scope)
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

AnyNode *createAssignNode(BaseNode *left, BaseNode *right)
{
    return new AnyNode(NodeType::Assign, [left = BaseNode::Ptr(left), right = BaseNode::Ptr(right)](Scope &scope)
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
        BaseObject *objectLHS = left->evaluate(scope);

        // Object we want to assign to LHS.
        // NB: evaluate in temporary scope so destroy when we exit this function.
        Scope tmpScope(scope);
        BaseObject *objectRHS = right->evaluate(tmpScope);

        // Update directly. TODO: - We will need to implement this for some object types still.
        *objectLHS = *objectRHS;
        return nullptr;
    });
}


AnyNode *createArrayNode(BaseNodeSharedPtrVector nodes)
{
    return new AnyNode(NodeType::Array, [nodes](Scope &scope)
    {
        BaseObjectPtrVector evaluatedObjects;

        evaluatedObjects.reserve(nodes.size());

        /* Scope owns all objects in array we are passing in. The array object will copy these! */
        for (auto &node : nodes)
        {
            evaluatedObjects.push_back(node->evaluate(scope));
        }

        return scope.createManagedObject<ArrayObject>(std::move(evaluatedObjects));
    });
}

AnyNode *createFileNode(BaseNodeSharedPtrVector nodes)
{
    return new AnyNode(NodeType::File, [nodes](Scope &scope)
    {
        for (const auto &node : nodes)
        {
            (void)node->evaluate(scope);
        }

        return nullptr;
    });
}


AnyNode *createPrefixIncrementNode(BaseNode *expression)
{
    return new AnyNode(NodeType::PrefixIncrement, [expression = BaseNode::Ptr(expression)](Scope &scope)
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


AnyNode *createPrefixDecrementNode(BaseNode *expression)
{
    return new AnyNode(NodeType::PrefixDecrement, [expression = BaseNode::Ptr(expression)](Scope &scope)
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


AnyNode *createNegationNode(BaseNode *expression)
{
    return new AnyNode(NodeType::Negation, [expression = BaseNode::Ptr(expression)](Scope &scope)
    {
        auto bodyEvaluated = expression->evaluate(scope);

        BaseObject *result{nullptr};

        if (bodyEvaluated->isObjectType<IntObject>())
            result = scope.createManagedObject<IntObject>(-bodyEvaluated->castObject<IntObject>());
        else if (bodyEvaluated->isObjectType<FloatObject>())
            result = scope.createManagedObject<FloatObject>(-bodyEvaluated->castObject<FloatObject>());
        else
            ThrowException("invalid object type");

        return result;
    });
}


AnyPropertyNode *createStructAccessNode(std::string structVarName, std::string memberVarName)
{
    auto evaluateNoClone = [structVarName, memberVarName](Scope &scope)
    {
        StructObject *structObject = scope.getNamedObject<StructObject>(structVarName);

        return structObject->instanceScope().getNamedObject(memberVarName);
    };

    auto evaluate = [evaluateNoClone](Scope &scope)
    {
        BaseObject *currentObject = evaluateNoClone(scope);
        return scope.cloneObject(currentObject);
    };


    return new AnyPropertyNode(NodeType::StructAccess, std::move(evaluate), std::move(evaluateNoClone));
}


AnyPropertyNode *createArrayAccessNode(BaseNode *arrayLookupNode, BaseNode *arrayIndexNode)
{
    assert(arrayLookupNode->isNodeType(NodeType::LookupVariable));

    auto evaluateNoClone = [arrayLookupNode = BaseNode::Ptr(arrayLookupNode),
                            arrayIndexNode = BaseNode::Ptr(arrayIndexNode)](Scope &scope)
    {
        // Lookup in array.
        auto &arrayObj = arrayLookupNode->evaluate(scope)->castObject<ArrayObject>();

        return arrayObj[arrayIndexNode->evaluateObject<IntObject::Type>(scope)];
    };

    auto evaluate = [evaluateNoClone](Scope &scope)
    {
        BaseObject *currentObject = evaluateNoClone(scope);
        return scope.cloneObject(currentObject);
    };

    return new AnyPropertyNode(NodeType::ArrayAccess, std::move(evaluate), std::move(evaluateNoClone));
}


AnyNode *createModuleNode(std::string moduleName, std::vector<ModuleFunctionPair> moduleFunctions)
{
    return new AnyNode(NodeType::Module, [moduleName = std::move(moduleName), moduleFunctions = std::move(moduleFunctions)](Scope &scope)
    {
        for (auto &it : moduleFunctions) /* Add to scope */
        {
            auto *object = scope.createManagedObject<ModuleFunctionObject>(it.second);
            scope.linkObject(it.first, object);
        }

        return nullptr;
    });
}

AnyNode *createClassMethodCallNode(std::string instanceName, FunctionCallNode *methodCallNode)
{
    return new AnyNode(NodeType::ClassMethodCall, [instanceName = std::move(instanceName),
                                                   methodCallNode = FunctionCallNode::Ptr(methodCallNode)](Scope &scope)
    {
        ClassObject *thisObject = scope.getNamedObject<ClassObject>(instanceName);

        // Important: to correctly evaluate the method, we need to add a parent scope
        // for the class instance temporarily each time we evaluate so function has
        // access to stuff created outside class.
        thisObject->instanceScope().setParentScope(&scope);

        /* But evaluate in class' instance scope */
        BaseObject *result = methodCallNode->evaluate(thisObject->instanceScope());

        // Set back to avoid problems if we forget to reset it in future.
        thisObject->instanceScope().setParentScope(nullptr);

        return result;
    });
}


} // namespace NodeFactory
