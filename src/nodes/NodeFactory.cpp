/**
 * @file NodeFactory.cpp
 * @author Edward Palmer
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "NodeFactory.hpp"
#include "IntObject.hpp"


namespace NodeFactory
{

AnyNode *createBoolNode(bool state)
{
    return new AnyNode([state](Scope &scope)
    {
        return scope.createManagedObject<BoolObject>(state);
    });
}

AnyNode *createIntNode(long value)
{
    return new AnyNode([value](Scope &scope)
    {
        return scope.createManagedObject<IntObject>(value);
    });
}


} // namespace NodeFactory