/**
 * @file StructNode.cpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructNode.hpp"
#include "EucleiaUtility.hpp"
#include "StructDefinitionObject.hpp"
#include "StructObject.hpp"

BaseObject *StructNode::evaluate(Scope &scope)
{
    // Get struct template.
    StructDefinitionObject *templateObj = scope.getNamedObject<StructDefinitionObject>(typeName);

    // Create and owned by scope.
    StructObject *structObj = scope.createManagedObject<StructObject>(templateObj);
    scope.linkObject(name, structObj);

    return structObj;
}


BaseObject *StructAccessNode::evaluateNoClone(Scope &scope)
{
    StructObject *structObject = scope.getNamedObject<StructObject>(name);

    return structObject->instanceScope.getNamedObject(memberVariableName);
}


BaseObject *StructAccessNode::evaluate(Scope &scope)
{
    BaseObject *currentObject = evaluateNoClone(scope);
    return scope.cloneObject(currentObject);
}
