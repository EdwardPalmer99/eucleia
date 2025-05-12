/**
 * @file StructAccessNode.cpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructAccessNode.hpp"
#include "Exceptions.hpp"
#include "Logger.hpp"
#include "StructDefinitionObject.hpp"
#include "StructObject.hpp"


BaseObject *StructAccessNode::evaluateNoClone(Scope &scope)
{
    _trace;

    StructObject *structObject = scope.getNamedObject<StructObject>(instanceName);

    return structObject->instanceScope.getNamedObject(memberName);
}


BaseObject *StructAccessNode::evaluate(Scope &scope)
{
    _trace;

    BaseObject *currentObject = evaluateNoClone(scope);
    return scope.cloneObject(currentObject);
}
