/**
 * @file StructDefinitionNode.cpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "StructDefinitionNode.hpp"
#include "EucleiaUtility.hpp"
#include "StructDefinitionObject.hpp"
#include <map>

BaseObject *StructDefinitionNode::evaluate(Scope &scope)
{
    auto &name = structTypeName->variableName;
    auto &programNodes = structMemberVars->programNodes;

    if (programNodes.empty())
    {
        EucleiaError("struct %s cannot be empty!\n", name.c_str());
        return nullptr;
    }

    std::map<std::string, StructDefinitionObject::SupportedObjectTypes> objectTypeForName;

    for (size_t iObject = 0; iObject < programNodes.size(); ++iObject)
    {
        // We need each node to be an AddVariableNode (i.e. int a; float b;).
        AddVariableNode &obj = programNodes[iObject]->castNode<AddVariableNode>();

        // Ensure each member variable name is unique.
        auto iter = objectTypeForName.find(obj.variableName);
        if (iter != objectTypeForName.end())
        {
            EucleiaError("duplicate member variable name %s in struct %s definition\n", obj.variableName.c_str(), name.c_str());
        }

        // Add to our map provided type is allowed for structs.
        // TODO: - have a single global enum class for object type.
        switch (obj.variableType)
        {
            case AddVariableNode::Int:
                objectTypeForName[obj.variableName] = StructDefinitionObject::SupportedObjectTypes::IntObject;
                break;
            case AddVariableNode::Float:
                objectTypeForName[obj.variableName] = StructDefinitionObject::SupportedObjectTypes::FloatObject;
                break;
            case AddVariableNode::String:
                objectTypeForName[obj.variableName] = StructDefinitionObject::SupportedObjectTypes::StringObject;
                break;
            default:
                EucleiaError("object type %s is not supported for structs.\n", obj.description().c_str());
                break;
        }
    }

    // Now create our struct definition and add to scope.
    StructDefinitionObject *obj = scope.createManagedObject<StructDefinitionObject>(objectTypeForName);
    scope.linkObject(name, obj);

    return obj;
}