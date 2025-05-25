/**
 * @file StructNode.hpp
 * @author Edward Palmer
 * @date 2024-11-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "BaseNode.hpp"

#include "Exceptions.hpp"
#include "Scope.hpp"
#include "StructDefinitionNode.hpp"
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>

/**
 * struct SomeStruct a;
 */
class StructNode : public BaseNode, public std::enable_shared_from_this<StructNode>
{
public:
    using Ptr = std::shared_ptr<StructNode>();

    StructNode() = delete;

    /**
     * Create a new empty StructNode with the name.
     */
    StructNode(std::string typeName_, std::string name_);

    /**
     * Assignment operator.
     */
    StructNode &operator=(const StructNode &other);

    // TODO: - we will need to copy this assignment check and add it to the AnyNode
    // type

    /**
     * Finishes initializing the struct object and links to the scope. Ownership
     * should pass to the scope from the AST. Returns a pointer to itself.
     */
    class AnyObject evaluate(Scope &scope) override;

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
     * Stores names of all variables stored in set.
     */
    std::unordered_set<std::string> variableNames;

    /**
     * Ony a single evaluate call allowed.
     */
    bool active{false};

    /**
     * Stores name of instance and the type.
     */
    std::string typeName;
    std::string name;

    /**
     * Store the struct definition once active.
     */
    std::shared_ptr<StructDefinitionNode> structDefinition{nullptr};
};
