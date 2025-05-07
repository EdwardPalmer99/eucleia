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


} // namespace NodeFactory