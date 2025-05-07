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
#include "StringObject.hpp"


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

AnyNode *createStringNode(std::string value)
{
    return new AnyNode([value = std::move(value)](Scope &scope)
    {
        return scope.createManagedObject<StringObject>(value);
    });
}


} // namespace NodeFactory