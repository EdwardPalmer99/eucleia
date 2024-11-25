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


BaseObject *StructAccessNode::evaluate(Scope &scope)
{
    StructObject *structObj = scope.getNamedObject<StructObject>(name);

    auto iter = structObj->objectForName.find(memberVariableName);
    if (iter == structObj->objectForName.end())
    {
        EucleiaError("No member variable named %s found for struct %s\n",
                     memberVariableName.c_str(), name.c_str());
    }

    // NB: - if we did NOT clone, then it would be a reference! Something to handle
    // later!

    // To avoid problems currently, we clone this value. NB: later, we will need
    // a way to assign values to the objects in each struct instance!
    BaseObject *clonedObject = iter->second->clone();

    // TODO: - NB: this is a memory leak.
    // Need to add to scope to manage this.
    return clonedObject;
}
