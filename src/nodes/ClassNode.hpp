/**
 * @file ClassNode.hpp
 * @author Edward Palmer
 * @date 2024-11-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "ClassDefinitionNode.hpp"
#include "Exceptions.hpp"
#include <memory>
#include <string>


/**
 * An instance of a class defined by the ClassDefinitionNode.
 */
class ClassNode : public BaseNode, public std::enable_shared_from_this<ClassNode>
{
public:
    using Ptr = std::shared_ptr<ClassNode>;

    ClassNode() = delete;
    ClassNode(std::string typeName_, std::string name_);

    ClassNode &operator=(const ClassNode &other);

    /**
     * Finishes initializing the class object and links to the scope.
     * @param scope The scope in which to add the instance.
     * @return BaseObject* Pointer to itself
     */
    std::shared_ptr<class AnyObject> evaluate(Scope &scope) override;

    [[nodiscard]] const Scope &instanceScope() const { return _instanceScope; }

    [[nodiscard]] Scope &instanceScope() { return _instanceScope; }

protected:
    /**
     * The struct has its own scope for storing its own variables. It does not
     * inherit from any parent scopes. Literally just used for storing stuff.
     * When this instance goes out of scope, all variables will be deleted.
     */
    Scope _instanceScope;

    /**
     * Stores name of instance and the type.
     */
    std::string typeName;
    std::string name;

    /**
     * Store the struct definition once active.
     */
    std::shared_ptr<ClassDefinitionNode> classDefinition{nullptr};
};