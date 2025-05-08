/**
 * @file NodeFactory.cpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "NodeFactory.hpp"
#include "AnyNode.hpp"
#include "BaseObject.hpp"
#include "ExpressionScope.hpp"
#include "FloatObject.hpp"
#include "IntObject.hpp"
#include "JumpPoints.hpp"
#include "NodeType.hpp"
#include "Scope.hpp"
#include "StringObject.hpp"
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

} // namespace NodeFactory